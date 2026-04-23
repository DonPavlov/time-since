#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>

void wifi_module_init(void);

void wifi_sync_start(void);
bool wifi_sync_tick(void);
bool wifi_sync_in_progress(void);
bool wifi_sync_done(void);
bool wifi_rtc_updated(void);
bool wifi_is_active(void);

/**
 * When true, NTP success does NOT auto-disconnect WiFi. Used while the
 * web log server is running so clients can reach it. Default: false.
 */
void wifi_set_keep_connected(bool keep);

void wifi_disconnect(void);

#endif /* WIFI_H */
