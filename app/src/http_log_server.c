/*
 * Minimal HTTP server exposing the in-RAM log ring buffer.
 *
 *  GET /       : HTML page with the latest log snapshot plus auto-refresh
 *  GET /logs   : text/plain snapshot of the rolling log window
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "http_log_server.h"
#include "log_backend_web.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(http_log, LOG_LEVEL_INF);

static const uint16_t http_port = 80;
static const char index_html_head[] =
	"<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
	"<title>time-since-box logs</title>"
	"<style>body{font-family:monospace;background:#111;color:#ddd;margin:0;padding:1em}"
	"a{color:#8cc2ff}pre{white-space:pre-wrap;word-break:break-word}</style></head>"
	"<body><h3>time-since-box — recent log</h3>"
	"<p><a href=\"/logs\">Open raw /logs</a></p><pre id=\"l\">";
static const char index_html_tail[] =
	"</pre><script>"
	"async function r(){try{const t=await(await fetch('/logs')).text();"
	"const e=document.getElementById('l');const b=e.scrollHeight-e.scrollTop-e.clientHeight<32;"
	"e.textContent=t;if(b)window.scrollTo(0,document.body.scrollHeight);}catch(e){}}"
	"setInterval(r,2000);"
	"</script></body></html>";

static K_MUTEX_DEFINE(server_lock);
static K_SEM_DEFINE(server_start_sem, 0, 1);

static int listen_fd = -1;
static bool server_enabled;

static uint8_t logs_scratch[4096];
static uint8_t page_scratch[12288];

static size_t append_str(uint8_t *dst, size_t pos, size_t max, const char *src)
{
	while (*src != '\0' && pos < max) {
		dst[pos++] = (uint8_t)*src++;
	}

	return pos;
}

static size_t append_escaped(uint8_t *dst, size_t pos, size_t max,
			     const uint8_t *src, size_t len)
{
	for (size_t i = 0; i < len && pos < max; i++) {
		switch (src[i]) {
		case '&':
			pos = append_str(dst, pos, max, "&amp;");
			break;
		case '<':
			pos = append_str(dst, pos, max, "&lt;");
			break;
		case '>':
			pos = append_str(dst, pos, max, "&gt;");
			break;
		default:
			dst[pos++] = src[i];
			break;
		}
	}

	return pos;
}

static bool http_server_is_enabled(void)
{
	bool enabled;

	k_mutex_lock(&server_lock, K_FOREVER);
	enabled = server_enabled;
	k_mutex_unlock(&server_lock);

	return enabled;
}

static void http_server_set_listen_fd(int fd)
{
	k_mutex_lock(&server_lock, K_FOREVER);
	listen_fd = fd;
	k_mutex_unlock(&server_lock);
}

static int http_server_sendall(int fd, const void *buf, size_t len)
{
	const uint8_t *ptr = buf;

	while (len > 0) {
		ssize_t out = zsock_send(fd, ptr, len, 0);

		if (out < 0) {
			return -errno;
		}

		ptr += out;
		len -= out;
	}

	return 0;
}

static size_t http_server_build_page(void)
{
	size_t log_len = log_backend_web_snapshot(logs_scratch, sizeof(logs_scratch));
	size_t pos = 0;

	pos = append_str(page_scratch, pos, sizeof(page_scratch), index_html_head);
	pos = append_escaped(page_scratch, pos, sizeof(page_scratch),
			     logs_scratch, log_len);
	pos = append_str(page_scratch, pos, sizeof(page_scratch), index_html_tail);

	return pos;
}

static int http_server_reply(int client_fd, const char *content_type,
			     const void *body, size_t body_len)
{
	char header[256];
	int header_len;

	header_len = snprintk(header, sizeof(header),
			      "HTTP/1.1 200 OK\r\n"
			      "Connection: close\r\n"
			      "Content-Type: %s\r\n"
			      "Content-Length: %u\r\n"
			      "\r\n",
			      content_type, (unsigned int)body_len);
	if (header_len <= 0 || header_len >= sizeof(header)) {
		return -ENOMEM;
	}

	if (http_server_sendall(client_fd, header, header_len) < 0) {
		return -EIO;
	}

	if (body_len > 0 && http_server_sendall(client_fd, body, body_len) < 0) {
		return -EIO;
	}

	return 0;
}

static void http_server_handle_client(int client_fd)
{
	char req[256];
	ssize_t len = zsock_recv(client_fd, req, sizeof(req) - 1, 0);

	if (len <= 0) {
		return;
	}

	req[len] = '\0';

	if (strncmp(req, "GET /logs", 9) == 0) {
		size_t body_len = log_backend_web_snapshot(logs_scratch,
							 sizeof(logs_scratch));
		(void)http_server_reply(client_fd, "text/plain; charset=utf-8",
					logs_scratch, body_len);
		return;
	}

	size_t body_len = http_server_build_page();
	(void)http_server_reply(client_fd, "text/html; charset=utf-8",
				page_scratch, body_len);
}

static int http_server_open_socket(void)
{
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(http_port),
		.sin_addr = { .s_addr = htonl(INADDR_ANY) },
	};
	struct zsock_pollfd pfd;
	int fd;
	int ret;

	fd = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0) {
		return -errno;
	}

	ret = zsock_setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
			       &(int){1}, sizeof(int));
	if (ret < 0) {
		ret = -errno;
		zsock_close(fd);
		return ret;
	}

	ret = zsock_bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		ret = -errno;
		zsock_close(fd);
		return ret;
	}

	ret = zsock_listen(fd, 1);
	if (ret < 0) {
		ret = -errno;
		zsock_close(fd);
		return ret;
	}

	http_server_set_listen_fd(fd);

	LOG_INF("HTTP log viewer listening on http://<device-ip>:%u/", http_port);

	pfd.fd = fd;
	pfd.events = ZSOCK_POLLIN;
	pfd.revents = 0;

	while (http_server_is_enabled()) {
		ret = zsock_poll(&pfd, 1, 500);
		if (ret < 0) {
			ret = -errno;
			break;
		}

		if (ret == 0 || !(pfd.revents & ZSOCK_POLLIN)) {
			continue;
		}

		int client_fd = zsock_accept(fd, NULL, NULL);
		if (client_fd < 0) {
			ret = -errno;
			break;
		}

		http_server_handle_client(client_fd);
		(void)zsock_close(client_fd);
	}

	http_server_set_listen_fd(-1);
	(void)zsock_close(fd);

	return ret;
}

static void http_log_server_thread(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	while (true) {
		k_sem_take(&server_start_sem, K_FOREVER);

		while (http_server_is_enabled()) {
			int ret = http_server_open_socket();

			if (!http_server_is_enabled()) {
				break;
			}

			LOG_ERR("HTTP log server stopped unexpectedly: %d", ret);
			k_sleep(K_MSEC(500));
		}
	}
}

K_THREAD_DEFINE(http_log_server_tid, 3072, http_log_server_thread,
		NULL, NULL, NULL, K_LOWEST_APPLICATION_THREAD_PRIO, 0, 0);

int http_log_server_start(void)
{
	k_mutex_lock(&server_lock, K_FOREVER);
	if (server_enabled) {
		k_mutex_unlock(&server_lock);
		return -EALREADY;
	}

	server_enabled = true;
	k_mutex_unlock(&server_lock);

	k_sem_give(&server_start_sem);
	return 0;
}

int http_log_server_stop(void)
{
	int fd_to_close = -1;

	k_mutex_lock(&server_lock, K_FOREVER);
	if (!server_enabled) {
		k_mutex_unlock(&server_lock);
		return -EALREADY;
	}

	server_enabled = false;
	fd_to_close = listen_fd;
	listen_fd = -1;
	k_mutex_unlock(&server_lock);

	if (fd_to_close >= 0) {
		(void)zsock_close(fd_to_close);
	}

	LOG_INF("HTTP log viewer stopped");
	return 0;
}
