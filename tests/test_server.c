#include "server.h"
#include <stdio.h>

START_TEST (test_octo_server_create)
{
    octo_server server;

    struct ev_loop *loop = EV_DEFAULT;
   
    int fd = fileno(stdin);
    octo_server_init(&server, loop, fd); 
    
    fail_unless(server.fd == fd,
        "fd not set correctly by server_init");

    octo_server_destroy(&server);

    fail_unless(server.fd == -1,
        "fd not reset by server_destroy");
}
END_TEST

START_TEST (test_octo_server_start)
{
    octo_server server;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_server_init(&server, loop, 0); 
    
    /* setup a bogus unix socket */

    /* serve on the socket */
    octo_server_serve(&server, );

    /* loop at least once */

    /* destroy */
    octo_server_destroy(&server);
}
END_TEST



