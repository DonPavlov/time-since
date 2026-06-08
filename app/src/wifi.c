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
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/ethernet_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/sntp.h>
#include <zephyr/net/wifi_mgmt.h>

#include <esp_mac.h>

LOG_MODULE_REGISTER(wifi, LOG_LEVEL_DBG);

static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));

static volatile bool wifi_connected;
static K_SEM_DEFINE(wifi_connect_sem, 0, 1);

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;

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

static void wifi_event_handler(struct net_mgmt_event_callback *cb,
			       uint64_t mgmt_event, struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
		const struct wifi_status *status = (const struct wifi_status *)cb->info;
		if (status->status == 0) {
			LOG_INF("WiFi connected");
			wifi_connected = true;
			k_sem_give(&wifi_connect_sem);
		} else {
			LOG_ERR("WiFi connection failed: %d", status->status);
			wifi_connected = false;
			k_sem_give(&wifi_connect_sem);
		}
	} else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
		LOG_INF("WiFi disconnected");
		wifi_connected = false;
		/* Wake a pending connect wait so it fails fast instead of
		 * blocking the full per-network timeout.
		 */
		k_sem_give(&wifi_connect_sem);
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
			LOG_INF("IP: %s", buf);
		}
	}
}

void wifi_module_init(void)
{
	wifi_apply_stable_mac();

	net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT |
				     NET_EVENT_WIFI_DISCONNECT_RESULT);
	net_mgmt_add_event_callback(&wifi_cb);

	net_mgmt_init_event_callback(&ipv4_cb, ipv4_addr_handler,
				     NET_EVENT_IPV4_ADDR_ADD);
	net_mgmt_add_event_callback(&ipv4_cb);
}

static int try_connect(struct net_if *iface, const struct wifi_network *net,
		       uint32_t timeout_ms)
{
	struct wifi_connect_req_params wifi_params = {
		.ssid = net->ssid,
		.ssid_length = strlen(net->ssid),
		.security = net->security,
		.channel = WIFI_CHANNEL_ANY,
		.band = WIFI_FREQ_BAND_2_4_GHZ,
		.mfp = WIFI_MFP_OPTIONAL,
		.timeout = SYS_FOREVER_MS,
	};

	if (net->security != WIFI_SECURITY_TYPE_NONE && net->password != NULL) {
		wifi_params.psk = net->password;
		wifi_params.psk_length = strlen(net->password);
	}

	LOG_INF("Trying SSID: %s", net->ssid);

	k_sem_reset(&wifi_connect_sem);

	int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params,
			   sizeof(wifi_params));
	if (ret != 0) {
		LOG_ERR("WiFi connect request failed: %d", ret);
		return ret;
	}

	if (k_sem_take(&wifi_connect_sem, K_MSEC(timeout_ms)) != 0) {
		LOG_WRN("Connect timeout for %s", net->ssid);
		(void)net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
		return -ETIMEDOUT;
	}

	return wifi_connected ? 0 : -ECONNREFUSED;
}

static void enable_modem_sleep(struct net_if *iface)
{
	struct wifi_ps_params params = {
		.enabled = WIFI_PS_ENABLED,
	};
	int ret = net_mgmt(NET_REQUEST_WIFI_PS, iface, &params, sizeof(params));

	if (ret != 0) {
		LOG_WRN("WiFi PS enable failed: %d", ret);
	} else {
		LOG_INF("WiFi modem sleep enabled");
	}
}

int wifi_connect_any(uint32_t per_network_timeout_ms)
{
	struct net_if *iface = net_if_get_default();

	if (!iface || KNOWN_NETWORKS_COUNT == 0) {
		return -ENODEV;
	}

	for (size_t i = 0; i < KNOWN_NETWORKS_COUNT; i++) {
		if (try_connect(iface, &known_networks[i],
				per_network_timeout_ms) == 0) {
			enable_modem_sleep(iface);
			return 0;
		}
	}

	LOG_WRN("All %u networks failed", (unsigned int)KNOWN_NETWORKS_COUNT);
	return -ETIMEDOUT;
}

int wifi_sync_ntp(int max_attempts, uint32_t retry_delay_seconds,
		  wifi_ntp_retry_cb_t retry_cb, void *user_data)
{
	struct sntp_time sntp_time;
	struct tm *utc;
	struct rtc_time rtc_time;
	time_t ts;
	int ret = -EIO;

	for (int attempt = 0; attempt < max_attempts; attempt++) {
		LOG_INF("NTP attempt %d/%d", attempt + 1, max_attempts);
		ret = sntp_simple("time.cloudflare.com", 3000, &sntp_time);
		if (ret == 0) {
			break;
		}
		LOG_WRN("NTP failed: %d", ret);
		if (attempt + 1 < max_attempts) {
			if (retry_cb != NULL) {
				retry_cb(ret, user_data);
			}
			k_sleep(K_SECONDS(retry_delay_seconds));
		}
	}

	if (ret != 0) {
		return ret;
	}

	ts = (time_t)sntp_time.seconds;
	utc = gmtime(&ts);
	if (utc == NULL) {
		return -EINVAL;
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

	if (!device_is_ready(rtc)) {
		return -ENODEV;
	}

	ret = rtc_set_time(rtc, &rtc_time);
	if (ret == 0) {
		LOG_INF("RTC set UTC: %04d-%02d-%02d %02d:%02d:%02d",
			rtc_time.tm_year + 1900, rtc_time.tm_mon + 1,
			rtc_time.tm_mday, rtc_time.tm_hour,
			rtc_time.tm_min, rtc_time.tm_sec);
	}

	return ret;
}

bool wifi_is_connected(void)
{
	return wifi_connected;
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
