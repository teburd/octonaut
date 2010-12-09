/**
 * Copyright (c) 2010 Tom Burdick <thomas.burdick@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "http_request.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


/**
 * http_parser callbacks
 */
static int request_message_begin(http_parser *parser)
{
    return 0;
}

static int request_path(http_parser *parser,
    const char *path, const size_t len)
{
    return 0;
}

static int request_query(http_parser *parser,
    const char *query, const size_t len)
{
    return 0;
}

static int request_url(http_parser *parser,
    const char *url,
    const size_t len)
{
    return 0;
}

static int request_fragment(http_parser *parser,
    const char *fragment,
    const size_t len)
{
    return 0;
}

static int request_header_field(http_parser *parser,
    const char *field,
    const size_t field len)
{
    return 0;
}

static int request_header_value(http_parser *parser,
    const char *value,
    const size_t len)
{
    return 0;
}

static int request_headers_complete(http_parser *parser)
{
    return 0;
}

static int request_body(http_parser *parser,
    const char *body,
    const size_t len)
{
    return 0;
}

static int request_message_complete(http_parser *parser)
{
    /**
     * pass the request to the callback then free() the memory used
     */
    return 0;
}

static http_parser_settings
parser_settings = {
    .on_message_begin       = request_message_begin,
    .on_path                = request_path,
    .on_query_string        = request_query,
    .on_url                 = request_url,
    .on_fragment            = request_fragment,
    .on_header_field        = request_header_field,
    .on_header_value        = request_header_value,
    .on_headers_complete    = request_headers_complete,
    .on_body                = request_body,
    .on_message_complete    = request_message_complete
};

/**
 * http_request callbacks
 */
static void request_read(void *ctx, uint8_t *data, size_t len)
{
    octo_http_connection *conn = (octo_http_connection *)ctx;
    http_parser_execute(conn->http_parser, data, len);
}

static void request_timer(EV_P_ ev_timer *watcher, int revents)
{
    /**
     * timeout occured
     */
    octo_http_request *request = ptr_offset(watcher, timer, octo_http_request);
    octo_aio_destroy(&conn->aio);
    ev_timer_stop(&conn->timer);
    free(conn);
}

void octo_http_request(int fd, struct sockaddr_in *addr, octo_http_request_cb cb)
{
    octo_http_request *request = malloc(sizeof(octo_http_request));
    octo_hash_default_init(
}

