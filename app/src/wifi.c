/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "wifi.h"
#include "wifi_creds.h"

#include <errno.h>
#include <string.h>
#include <time.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/sntp.h>
#include <zephyr/net/wifi_mgmt.h>

#include <esp_mac.h>

#include "time_utils.h"

LOG_MODULE_REGISTER(wifi, LOG_LEVEL_DBG);

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

static volatile bool wifi_connected;
static volatile bool rtc_updated;
static volatile bool keep_connected;
static volatile size_t wifi_network_idx;

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;

enum sync_state {
	SYNC_IDLE,
	SYNC_CONNECTING,
	SYNC_NTP,
	SYNC_DONE,
};

static volatile enum sync_state sync_state = SYNC_IDLE;
static uint32_t sync_timer;
static int ntp_attempts;

static void log_iface_mac(struct net_if *iface, const char *prefix)
{
	const struct net_linkaddr *link_addr;

	if (iface == NULL) {
		return;
	}

	link_addr = net_if_get_link_addr(iface);
	if (link_addr == NULL || link_addr->len < WIFI_MAC_ADDR_LEN) {
		return;
	}

	LOG_INF("%s%02x:%02x:%02x:%02x:%02x:%02x",
		prefix,
		link_addr->addr[0], link_addr->addr[1], link_addr->addr[2],
		link_addr->addr[3], link_addr->addr[4], link_addr->addr[5]);
}

static void wifi_apply_stable_mac(void)
{
	struct net_if *iface = net_if_get_default();
	struct ethernet_req_params params = { 0 };
	uint8_t factory_mac[WIFI_MAC_ADDR_LEN];
	int ret;

	if (iface == NULL) {
		LOG_WRN("No default network interface, skipping MAC setup");
		return;
	}

	ret = esp_efuse_mac_get_default(factory_mac);
	if (ret != ESP_OK) {
		LOG_ERR("Failed to read factory MAC: %d", ret);
		return;
	}

	memcpy(params.mac_address.addr, factory_mac, sizeof(factory_mac));

	ret = net_mgmt(NET_REQUEST_ETHERNET_SET_MAC_ADDRESS, iface, &params,
		       sizeof(params));
	if (ret != 0) {
		LOG_ERR("Failed to apply stable WiFi MAC: %d", ret);
		return;
	}

	log_iface_mac(iface, "Using WiFi STA MAC: ");
}

static void wifi_connect_handler(struct net_mgmt_event_callback *cb,
				 uint64_t mgmt_event, struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
		const struct wifi_status *status = (const struct wifi_status *)cb->info;
		if (status->status == 0) {
			LOG_INF("WiFi connected successfully");
			wifi_connected = true;
		} else {
			LOG_ERR("WiFi connection failed: %d", status->status);
			wifi_connected = false;
		}
	} else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
		LOG_INF("WiFi disconnected");
		wifi_connected = false;
	}
}

static void ipv4_addr_handler(struct net_mgmt_event_callback *cb,
			      uint64_t mgmt_event, struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_IPV4_ADDR_ADD) {
		char buf[16];
		const struct in_addr *addr = net_if_ipv4_get_global_addr(iface,
									 NET_ADDR_PREFERRED);
		if (addr) {
			net_addr_ntop(AF_INET, addr, buf, sizeof(buf));
			LOG_INF("IP address obtained: %s", buf);
			log_iface_mac(iface, "Router should show MAC: ");
			LOG_INF("Logs: http://%s/ (hostname: time-since-box)", buf);
		} else {
			LOG_INF("IP address assigned");
		}
	}
}

static void start_wifi_connect(void)
{
	struct net_if *iface = net_if_get_default();
	struct wifi_connect_req_params wifi_params = { 0 };

	if (!iface || wifi_connected || KNOWN_NETWORKS_COUNT == 0) {
		return;
	}

	if (wifi_network_idx >= KNOWN_NETWORKS_COUNT) {
		wifi_network_idx = 0;
	}

	const struct wifi_network *net = &known_networks[wifi_network_idx];

	wifi_params.ssid = net->ssid;
	wifi_params.ssid_length = strlen(net->ssid);
	wifi_params.security = net->security;
	wifi_params.channel = WIFI_CHANNEL_ANY;
	wifi_params.band = WIFI_FREQ_BAND_2_4_GHZ;
	wifi_params.mfp = WIFI_MFP_OPTIONAL;
	wifi_params.timeout = SYS_FOREVER_MS;

	if (net->security != WIFI_SECURITY_TYPE_NONE && net->password != NULL) {
		wifi_params.psk = net->password;
		wifi_params.psk_length = strlen(net->password);
	}

	LOG_INF("Attempting to connect to WiFi: %s", net->ssid);
	int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params,
			   sizeof(struct wifi_connect_req_params));
	if (ret != 0) {
		LOG_ERR("WiFi connect request failed: %d", ret);
		wifi_network_idx++;
	}
}

static int try_ntp_sync(void)
{
	struct sntp_time sntp_time;
	struct tm *utc;
	struct rtc_time rtc_time;
	time_t ts;
	int ret;

	LOG_INF("Trying NTP sync...");
	ret = sntp_simple("time.cloudflare.com", 2000, &sntp_time);
	if (ret < 0) {
		LOG_WRN("NTP failed: %d", ret);
		return ret;
	}

	ts = (time_t)sntp_time.seconds;
	utc = gmtime(&ts);
	if (utc == NULL) {
		return -1;
	}

	rtc_time = (struct rtc_time) {
		.tm_year = utc->tm_year,
		.tm_mon = utc->tm_mon,
		.tm_mday = utc->tm_mday,
		.tm_hour = utc->tm_hour,
		.tm_min = utc->tm_min,
		.tm_sec = utc->tm_sec,
		.tm_wday = utc->tm_wday,
		.tm_yday = utc->tm_yday,
		.tm_isdst = 0,
	};

	if (device_is_ready(rtc)) {
		ret = rtc_set_time(rtc, &rtc_time);
		if (ret == 0) {
			LOG_INF("Stored RTC time in UTC: %04d-%02d-%02d %02d:%02d:%02d",
				rtc_time.tm_year + 1900, rtc_time.tm_mon + 1,
				rtc_time.tm_mday, rtc_time.tm_hour,
				rtc_time.tm_min, rtc_time.tm_sec);
			struct rtc_time readback = { 0 };
			if (rtc_get_time(rtc, &readback) == 0) {
				time_t readback_epoch = time_utils_rtc_to_epoch_utc(&readback);
				bool berlin_dst = time_utils_berlin_is_dst_utc(readback_epoch);
				int berlin_offset = berlin_dst ? 7200 : 3600;
				time_t berlin_epoch = readback_epoch + berlin_offset;
				struct tm *berlin = gmtime(&berlin_epoch);

				LOG_INF("Device RTC UTC: %04d-%02d-%02d %02d:%02d:%02d",
					readback.tm_year + 1900,
					readback.tm_mon + 1,
					readback.tm_mday, readback.tm_hour,
					readback.tm_min, readback.tm_sec);
				if (berlin != NULL) {
					LOG_INF("Berlin wall time: %04d-%02d-%02d %02d:%02d:%02d (%s)",
						berlin->tm_year + 1900,
						berlin->tm_mon + 1,
						berlin->tm_mday, berlin->tm_hour,
						berlin->tm_min, berlin->tm_sec,
						berlin_dst ? "CEST" : "CET");
				}
			}
			rtc_updated = true;
			return 0;
		}
	}

	return ret;
}

void wifi_module_init(void)
{
	wifi_apply_stable_mac();

	net_mgmt_init_event_callback(&wifi_cb, wifi_connect_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT |
				     NET_EVENT_WIFI_DISCONNECT_RESULT);
	net_mgmt_add_event_callback(&wifi_cb);

	net_mgmt_init_event_callback(&ipv4_cb, ipv4_addr_handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&ipv4_cb);
}

void wifi_sync_start(void)
{
	sync_state = SYNC_CONNECTING;
	sync_timer = 0;
	ntp_attempts = 0;
	rtc_updated = false;
	wifi_network_idx = 0;
	start_wifi_connect();
}

bool wifi_sync_tick(void)
{
	sync_timer++;

	switch (sync_state) {
	case SYNC_IDLE:
		break;

	case SYNC_CONNECTING:
		if (wifi_connected) {
			if (sync_timer >= 5) {
				LOG_INF("Trying NTP...");
				sync_state = SYNC_NTP;
				sync_timer = 0;
			}
		} else if (sync_timer >= 10) {
			wifi_network_idx++;
			if (wifi_network_idx >= KNOWN_NETWORKS_COUNT * 3) {
				LOG_WRN("WiFi connect failed after trying all networks");
				sync_state = SYNC_DONE;
				return true;
			}
			start_wifi_connect();
			sync_timer = 0;
		}
		break;

	case SYNC_NTP:
		if (sync_timer >= 3) {
			sync_timer = 0;
			if (try_ntp_sync() == 0) {
				if (!keep_connected) {
					wifi_disconnect();
				}
				sync_state = SYNC_DONE;
				return true;
			}
			ntp_attempts++;
			if (ntp_attempts >= 10) {
				LOG_WRN("NTP failed after %d attempts, giving up",
					ntp_attempts);
				if (!keep_connected) {
					wifi_disconnect();
				}
				sync_state = SYNC_DONE;
				return true;
			}
		}
		break;

	case SYNC_DONE:
		break;
	}

	return false;
}

bool wifi_sync_in_progress(void)
{
	return sync_state != SYNC_IDLE && sync_state != SYNC_DONE;
}

bool wifi_sync_done(void)
{
	return sync_state == SYNC_DONE;
}

bool wifi_rtc_updated(void)
{
	return rtc_updated;
}

bool wifi_is_active(void)
{
	return keep_connected || wifi_connected ||
	       (sync_state != SYNC_IDLE && sync_state != SYNC_DONE);
}

void wifi_set_keep_connected(bool keep)
{
	keep_connected = keep;
}

void wifi_disconnect(void)
{
	struct net_if *iface = net_if_get_default();

	if (!iface || !wifi_connected) {
		return;
	}

	int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
	if (ret != 0) {
		LOG_ERR("WiFi disconnect failed: %d", ret);
	}
	wifi_connected = false;
}
