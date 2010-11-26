#include "aio.h"

START_TEST (test_octo_aio_create)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_aio_init(&aio, loop, (int)stdin, 128); 

    printf("test_octo_aio_create\n");
}
END_TEST

START_TEST (test_octo_aio_destroy)
{
    printf("test_octo_aio_destroy\n");
}
END_TEST

TCase* octo_aio_tcase()
{
    TCase* tc_octo_aio = tcase_create("octo_aio");
    tcase_add_test(tc_octo_aio, test_octo_aio_create);
    tcase_add_test(tc_octo_aio, test_octo_aio_destroy);
    return tc_octo_aio;
}
