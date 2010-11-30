#include "buffer.h"

START_TEST (test_octo_buffer_init_destroy)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size not set correctly by buffer_init");

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_size)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_write)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST


START_TEST (test_octo_buffer_fwrite)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_read)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_fread)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_peek)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_drain)
{
    octo_buffer buf;

    octo_buffer_init(&buf); 

    octo_buffer_destroy(&buf);
}
END_TEST

TCase* octo_buffer_tcase()
{
    TCase* tc_octo_buffer = tcase_create("octo_buffer");
    tcase_add_test(tc_octo_buffer, test_octo_buffer_init_destroy);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_size);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_write);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_fwrite);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_peek);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_drain);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_read);
    tcase_add_test(tc_octo_buffer, test_octo_buffer_fread);
    return tc_octo_buffer;
}
