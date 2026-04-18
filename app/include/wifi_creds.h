#ifndef WIFI_CREDS_H
#define WIFI_CREDS_H

#include <zephyr/net/wifi_mgmt.h>

/**
 * @brief WiFi network credentials
 *
 * Struct definition lives here. The actual `known_networks[]` array and
 * `KNOWN_NETWORKS_COUNT` macro live in `secrets.h` (gitignored).
 * See `secrets.h.example` for the template.
 */
struct wifi_network {
	const char *ssid;
	const char *password;
	enum wifi_security_type security;
};

#include "secrets.h"

#endif /* WIFI_CREDS_H */
