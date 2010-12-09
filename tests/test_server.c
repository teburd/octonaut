#include "server.h"
#include <stdio.h>

typedef struct mock_server
{
    octo_server server;
    int errors;
    int connects;
} mock_server;

bool mock_server_connect(octo_server *server, int fd, struct sockaddr *addr, socklen_t len)
{
    octo_logger_debug(server->logger, "connect called");
    mock_server *mserver = ptr_offset(server, mock_server, server);
    mserver->connects += 1;
    close(fd);
    return true;
}

bool mock_server_error(octo_server *server)
{
    octo_logger_debug(server->logger, "error called");
    mock_server *mserver = ptr_offset(server, mock_server, server);
    mserver->connects += 1;
    mserver->errors += 1;
    return true;
}

START_TEST (test_octo_server_init)
{
    mock_server server;

    struct ev_loop *loop = EV_DEFAULT;

    octo_server_init(&server.server, loop, 1,
            mock_server_connect, mock_server_error); 
    server.errors = 0;
    server.connects = 0;
    
    octo_server_destroy(&server.server);
}
END_TEST

START_TEST (test_octo_server_serve)
{
    mock_server server;

    struct ev_loop *loop = EV_DEFAULT;
    octo_server_init(&server.server, loop, 1,
            mock_server_connect, mock_server_error); 

    /*

    int sockfd = ...
    octo_server_serve(&server, );

    */

    /* destroy */
    octo_server_destroy(&server.server);
}
END_TEST

TCase* octo_server_tcase()
{
    TCase* tc_octo_server = tcase_create("octo_server");
    tcase_add_test(tc_octo_server, test_octo_server_init);
    tcase_add_test(tc_octo_server, test_octo_server_serve);
    return tc_octo_server;
}


