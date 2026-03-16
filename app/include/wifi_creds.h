#ifndef WIFI_CREDS_H
#define WIFI_CREDS_H

#include <zephyr/net/wifi_mgmt.h>

/**
 * @brief WiFi network credentials
 *
 * Add or modify networks here to connect to known WiFi networks.
 */
struct wifi_network {
	const char *ssid;
	const char *password;
	enum wifi_security_type security;
};

/* Known WiFi networks */
static const struct wifi_network known_networks[] = {
	{
		.ssid = "Hein",
		.password = "paulhein1!",
		.security = WIFI_SECURITY_TYPE_PSK,
	},
	/* Add more networks here as needed */
};

#define KNOWN_NETWORKS_COUNT (sizeof(known_networks) / sizeof(known_networks[0]))

#endif /* WIFI_CREDS_H */
