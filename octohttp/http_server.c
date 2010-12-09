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

#include "http_server.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <octonaut/common.h>


void octo_http_server_init(octo_http_server *server, struct ev_loop *loop, int port,
    int backlog)
{
    server->port = port;
    server->backlog = backlog;
}

void octo_http_server_destroy(octo_http_server *server)
{
    close(server->fd);
    server->port = 0;
}

static void octo_http_server_accept(EV_P_ ev_io *watcher, int revents)
{
    octo_http_server *server = ptr_offset(watcher, accept_watcher, octo_http_server);
    struct sockaddr_in connection_addr;
    socklen_t conn_addr_len = sizeof(connection_addr);
    int connfd = accept(server->fd, (struct sockaddr *)&connection_addr, &conn_addr_len);

    if(connfd < 0)
    {
        perror("accept");
        return;
    }

    close(connfd);
}

bool octo_http_server_serve(octo_http_server *server)
{
    /* open a socket to listen on the given port */
    struct sockaddr_in serv_addr;
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server->fd < 0)
    {
        perror("socket");
        return false;
    }

    memset((char*) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(server->port);

    if(bind(server->fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        return false;
    }

    listen(server->fd, server->backlog);

    ev_io_init(&server->accept_watcher, octo_http_server_accept, server->fd, EV_READ);
    ev_io_start(server->loop, &server->accept_watcher);

    return true;
}
