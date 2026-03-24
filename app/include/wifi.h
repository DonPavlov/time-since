#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>

void wifi_module_init(void);

void wifi_sync_start(void);
bool wifi_sync_tick(void);
bool wifi_sync_in_progress(void);
bool wifi_sync_done(void);
bool wifi_rtc_updated(void);

void wifi_disconnect(void);

#endif /* WIFI_H */
