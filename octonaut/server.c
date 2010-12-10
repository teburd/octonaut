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
    printf("accept called\n");
    octo_server *server = ptr_offset(watcher, octo_server, read_watcher);
    struct sockaddr_storage addr;
    socklen_t len = sizeof(addr);

    printf("accepting on socket fd %d\n", server->fd);
    int connfd = accept(server->fd, (struct sockaddr *)&addr, &len);
    
    if(connfd < 0)
    {
        perror("accept");
        server->error(server);
    }
    else
    {
        server->connect(server, connfd, &addr, len);
    }
}

void octo_server_init(octo_server *server, struct ev_loop *loop,
    int backlog, octo_server_connect_cb connect, octo_server_error_cb error)
{
    octo_logger_init(&server->logger, "server");
    server->active = false;
    server->loop = loop;
    server->backlog = backlog;
    server->connect =  connect;
    server->error = error;
    server->active = false;
}

void octo_server_destroy(octo_server *server)
{

    if(server->active)
    {
        ev_io_stop(server->loop, &server->read_watcher);
        server->active = false;
    }
    octo_logger_debug(server->logger, "destroyed server");
    octo_logger_destroy(&server->logger);
}


bool octo_server_serve(octo_server *server, int fd)
{
    int result = 0;

    octo_logger_info(server->logger, "serving");
    server->fd = fd;

    result = listen(server->fd, server->backlog);
    if(result < 0)
    {
        octo_logger_error(server->logger, "listen() failed, %s", strerror(errno));
        return false;
    }

    ev_io_init(&server->read_watcher, octo_server_accept, server->fd, EV_READ);
    ev_io_start(server->loop, &server->read_watcher);

    server->active = true;

    return true;
}

bool octo_server_isactive(octo_server *server)
{
    return server->active;
}

