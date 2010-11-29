#include "buffer.h"

START_TEST (test_octo_buffer_create)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size not set correctly by buffer_init");

    octo_buffer_destroy(&buf);
}
END_TEST

TCase* octo_buffer_tcase()
{
    TCase* tc_octo_buffer = tcase_create("octo_buffer");
    tcase_add_test(tc_octo_buffer, test_octo_buffer_create);
    return tc_octo_buffer;
}
