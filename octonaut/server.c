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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.h"

#include "server.h"

static void octo_server_accept(EV_P_ ev_io *watcher, int revents)
{
    octo_server *server = ptr_offset(watcher, read_watcher, octo_server);
    struct sockaddr_in connection_addr;
    socklen_t conn_addr_len = sizeof(connection_addr);
    int connfd = accept(server->fd, (struct sockaddr *)&connection_addr, &conn_addr_len);

    if(connfd < 0)
    {
        perror("accept");
        return;
    }

}

void octo_server_init(octo_server *server, struct ev_loop *loop,
    int backlog, octo_server_connect_cb connect, octo_server_error_cb error)
{
    server->loop = loop;
    server->fd = -1;
    server->backlog = backlog;
    server->connect =  connect;
    server->error = error;
}

void octo_server_destroy(octo_server *server)
{
    if(ev_is_active(&server->read_watcher))
    {
        ev_io_stop(server->loop, &server->read_watcher);
    }

    octo_logger_debug(server->logger, "destroyed server");
}


bool octo_server_serve(octo_server *server, int fd)
{
    int result = 0;

    octo_logger_info(server->logger, "serving");

    result = listen(server->fd, server->backlog);
    if(result < 0)
    {
        octo_logger_error(server->logger, "listen() failed, %s", strerror(errno));
        return false;
    }

    ev_io_init(&server->read_watcher, octo_server_accept, server->fd, EV_READ);
    ev_io_start(server->loop, &server->read_watcher);

    return true;
}

bool octo_server_isactive(octo_server *server)
{
    return ev_is_active(&server->read_watcher);
}
