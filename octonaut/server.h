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

#ifndef OCTO_SERVER_H
#define OCTO_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>

#include <ev.h>

#include "logger.h"


typedef struct octo_server octo_server;

typedef void (* octo_server_connect_cb)(octo_server *server, int fd,
    struct sockaddr addr, socklen_t addr_len);
typedef void (* octo_server_error_cb)(octo_server *server,  int error);

struct octo_server
{
    struct ev_loop *loop;
    octo_logger logger;
    int fd;
    int backlog;
    ev_io read_watcher;
    octo_server_connect_cb connect;
    octo_server_error_cb error;
};

/**
 * passing in a bound socket results in a listening server
 * which will call connect when ever a new connection
 * has been accepted and error if an error occurs.
 *
 * Some errors may cause the server to become invalid.
 * The isactive function should be used to check in such
 * circumstances.
 *
 * Destroying the server results in a octo_server struct
 * that can be reused or freed as desired.
 */

void octo_server_init(octo_server *server, struct ev_loop *loop,
    int backlog, octo_server_connect_cb connect,
    octo_server_error_cb error);
bool octo_server_serve(octo_server *server, int fd);
bool octo_server_isactive(octo_server *server);
void octo_server_destroy(octo_server *server);

#endif
