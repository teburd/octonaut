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

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void octo_http_server_init(http_server *server, struct ev_loop *loop, int port,
    int backlog)
{
    server->port = port;
    server->backlog = backlog;
}

void octo_http_server_destroy(http_server *server)
{
    octo_aio_destroy(&server->aio);
    server->port = 0;
}

static void octo_http_server_accept(EV_P_ ev_io *watcher, int revents)
{
    octo_http_server *server = OFFSET(watcher, accept_watcher, octo_http_server);
    struct sockaddr_in connection_addr;
    int connfd = accept(server->fd, &connection_addr, sizeof(connection_addr));

    if(connfd < 0)
    {
        perror("accept");
        return;
    }
    
    octo_http_connection *conn = malloc(sizeof(octo_http_connection));

    octo_aio_init(&conn->aio, watcher->loop, connfd);
    conn->read_cb = octo_http_connection_read;
    conn->read_ctx = conn;

    ev_timer_init(&conn->http_timeout, octo_http_connection_timeout, 20.0, 0.0);
    ev_timer_start(watcher->loop, &conn->http_timeout);
}

bool octo_http_server_serve(http_server *server)
{
    /* open a socket to listen on the given port */
    struct sockaddr_in serv_addr;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)
    {
        perror("socket");
        return false;
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(server->port);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        return false;
    }

    listen(sockfd, server->backlog);

    ev_io_init(&server->accept_watcher, octo_http_server_accept, sockfd, EV_READ);
    ev_io_start(server->loop, &server->accept_watcher);

    return true;
}
