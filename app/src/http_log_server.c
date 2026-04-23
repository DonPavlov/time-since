/*
 * HTTP server exposing the in-RAM log ring buffer.
 *
 *  GET /       : small HTML shell that auto-refreshes /logs every 2s
 *  GET /logs   : text/plain snapshot of the rolling log window
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "http_log_server.h"
#include "log_backend_web.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/http/method.h>
#include <zephyr/net/http/server.h>
#include <zephyr/net/http/service.h>

LOG_MODULE_REGISTER(http_log, LOG_LEVEL_INF);

static uint16_t http_port = 80;

static const char index_html[] =
	"<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
	"<title>time-since-box logs</title>"
	"<style>body{font-family:monospace;background:#111;color:#ddd;margin:0;padding:1em}"
	"pre{white-space:pre-wrap;word-break:break-all}</style></head>"
	"<body><h3>time-since-box — recent log</h3><pre id=\"l\">loading...</pre>"
	"<script>"
	"async function r(){try{const t=await(await fetch('/logs')).text();"
	"const e=document.getElementById('l');const b=e.scrollHeight-e.scrollTop-e.clientHeight<32;"
	"e.textContent=t;if(b)window.scrollTo(0,document.body.scrollHeight);}catch(e){}}"
	"r();setInterval(r,2000);"
	"</script></body></html>";

static struct http_resource_detail_static index_detail = {
	.common = {
		.type = HTTP_RESOURCE_TYPE_STATIC,
		.bitmask_of_supported_http_methods = BIT(HTTP_GET),
		.content_type = "text/html",
	},
	.static_data = index_html,
	.static_data_len = sizeof(index_html) - 1,
};

static uint8_t logs_scratch[4096];

static int logs_handler(struct http_client_ctx *client,
			enum http_transaction_status status,
			const struct http_request_ctx *request_ctx,
			struct http_response_ctx *response_ctx,
			void *user_data)
{
	ARG_UNUSED(client);
	ARG_UNUSED(request_ctx);
	ARG_UNUSED(user_data);

	if (status != HTTP_SERVER_REQUEST_DATA_FINAL) {
		return 0;
	}

	size_t n = log_backend_web_snapshot(logs_scratch, sizeof(logs_scratch));

	response_ctx->body = logs_scratch;
	response_ctx->body_len = n;
	response_ctx->final_chunk = true;
	return 0;
}

static struct http_resource_detail_dynamic logs_detail = {
	.common = {
		.type = HTTP_RESOURCE_TYPE_DYNAMIC,
		.bitmask_of_supported_http_methods = BIT(HTTP_GET),
		.content_type = "text/plain; charset=utf-8",
	},
	.cb = logs_handler,
	.user_data = NULL,
};

HTTP_SERVICE_DEFINE(log_http_service, NULL, &http_port, 1, 2, NULL, NULL, NULL);

HTTP_RESOURCE_DEFINE(index_resource, log_http_service, "/", &index_detail);
HTTP_RESOURCE_DEFINE(logs_resource, log_http_service, "/logs", &logs_detail);

int http_log_server_start(void)
{
	int ret = http_server_start();

	if (ret < 0) {
		LOG_ERR("http_server_start failed: %d", ret);
	} else {
		LOG_INF("HTTP log viewer listening on port %u", http_port);
	}
	return ret;
}

int http_log_server_stop(void)
{
	int ret = http_server_stop();

	if (ret < 0) {
		LOG_ERR("http_server_stop failed: %d", ret);
	} else {
		LOG_INF("HTTP log viewer stopped");
	}
	return ret;
}
