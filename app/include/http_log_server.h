#ifndef HTTP_LOG_SERVER_H
#define HTTP_LOG_SERVER_H

/**
 * Start the HTTP server that exposes the in-RAM log ring buffer.
 *
 * Idempotent — safe to call once WiFi is initialised. The HTTP server
 * background thread waits for DHCP to assign an IP before accept(2) succeeds.
 */
int http_log_server_start(void);

/**
 * Stop the HTTP server. After this, /logs is no longer reachable until the
 * next http_log_server_start() call.
 */
int http_log_server_stop(void);

#endif
