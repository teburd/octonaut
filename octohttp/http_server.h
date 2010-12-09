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

#ifndef OCTO_HTTP_SERVER_H
#define OCTO_HTTP_SERVER_H

#include <stdbool.h>
#include <ev.h>

#include "http_request.h"

typedef struct octo_http_server
{
    struct ev_loop *loop;
    int fd;
    int port;
    int backlog;
    ev_io accept_watcher;
    void *request_ctx;
    octo_http_request_cb cb;
} octo_http_server;

void octo_http_server_init(octo_http_server *server, struct ev_loop *loop,
    int port, int backlog, octo_http_request_cb cb, void *request_ctx);
void octo_http_server_destroy(octo_http_server *server);
bool octo_http_server_serve(octo_http_server *server);

#endif
