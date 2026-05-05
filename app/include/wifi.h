#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>
#include <stdint.h>

void wifi_module_init(void);

/**
 * Try each entry in known_networks[] once, with the given per-network
 * connect timeout. Returns 0 on success, -ETIMEDOUT if all networks fail.
 */
int wifi_connect_any(uint32_t per_network_timeout_ms);

/**
 * Try sntp_simple up to max_attempts times against time.cloudflare.com,
 * 3s timeout per attempt. On success, writes RTC. Returns 0 on success.
 */
int wifi_sync_ntp(int max_attempts);

bool wifi_is_connected(void);

void wifi_disconnect(void);

#endif /* WIFI_H */
