#include "aio.h"

START_TEST (test_octo_aio_create)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_aio_init(&aio, loop, (size_t)stdin, 128); 

    octo_aio_destroy(&aio);
}
END_TEST

START_TEST (test_octo_aio_start)
{
    octo_aio aio;

    struct ev_loop *loop = EV_DEFAULT;
    
    octo_aio_init(&aio, loop, 0, 128); 

    octo_aio_start(&aio);

    octo_aio_destroy(&aio);
}
END_TEST

TCase* octo_aio_tcase()
{
    TCase* tc_octo_aio = tcase_create("octo_aio");
    tcase_add_test(tc_octo_aio, test_octo_aio_create);
    tcase_add_test(tc_octo_aio, test_octo_aio_start);
    return tc_octo_aio;
}
