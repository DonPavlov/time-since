#ifndef WIFI_CREDS_H
#define WIFI_CREDS_H

/**
 * @brief WiFi network credentials
 *
 * Add or modify networks here to connect to known WiFi networks
 */

struct wifi_network {
	const char *ssid;
	const char *password;
	const char *security;  /* "open", "wpa2", "wpa3" */
};

/* Known WiFi networks */
static const struct wifi_network known_networks[] = {
	{
		.ssid = "Hein",
		.password = "paulhein1!",
		.security = "wpa2"
	},
	/* Add more networks here as needed */
};

#define KNOWN_NETWORKS_COUNT (sizeof(known_networks) / sizeof(known_networks[0]))

#endif /* WIFI_CREDS_H */
