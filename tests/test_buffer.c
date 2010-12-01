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
    const char mystr[] = "the world is not enough";
    char cmpstr[sizeof(mystr)];

    octo_buffer_init(&buf); 

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size is not correct");

    octo_buffer_write(&buf, (uint8_t*)mystr, sizeof(mystr));

    fail_unless(octo_buffer_size(&buf) == sizeof(mystr),
        "buffer size is not correct");

    octo_buffer_read(&buf, (uint8_t*)cmpstr, sizeof(mystr));

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size is not correct");

    octo_buffer_destroy(&buf);
}
END_TEST

START_TEST (test_octo_buffer_write)
{
    size_t len = 0;
    octo_buffer buf;
    const char mystr[] = "the world is not enough";

    octo_buffer_init(&buf); 

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size is not correct");

    for(int i = 0; i < 10; ++i)
    {
        len = octo_buffer_write(&buf, (uint8_t*)mystr, sizeof(mystr));

        fail_unless(len == sizeof(mystr),
            "buffer write did not return of string");
        
        fail_unless(octo_buffer_size(&buf) == (i+1)*sizeof(mystr),
            "buffer size is not correct");
    }
   
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
    size_t len = 0;
    octo_buffer buf;
    const char mystr[] = "the world is not enough";
    char cmpstr[sizeof(mystr)];

    octo_buffer_init(&buf); 

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size is not correct");

    for(int i = 0; i < 10; ++i)
    {
        len = octo_buffer_write(&buf, (uint8_t*)mystr, sizeof(mystr));

        fail_unless(len == sizeof(mystr),
            "buffer write did not return of string");
        
        fail_unless(octo_buffer_size(&buf) == (i+1)*sizeof(mystr),
            "buffer size is not correct");
    }

    for(int i = 0; i < 10; ++i)
    {
        len = octo_buffer_read(&buf, (uint8_t*)cmpstr, sizeof(mystr));

        fail_unless(len == sizeof(mystr),
            "buffer read did not return of string");
        
        fail_unless(octo_buffer_size(&buf) == (9-i)*sizeof(mystr),
            "buffer size is not correct");
       
        fail_unless(strncmp(cmpstr, mystr, sizeof(mystr)) == 0,
            "buffer read does not match expected string");
    }

    len = octo_buffer_write(&buf, (uint8_t*)mystr, sizeof(mystr));

    fail_unless(len == sizeof(mystr),
        "buffer write did not return correct length");

    fail_unless(octo_buffer_size(&buf) == sizeof(mystr),
        "buffer size is not correct");

    len = octo_buffer_read(&buf, (uint8_t*)cmpstr, 4);

    fail_unless(octo_buffer_size(&buf) == sizeof(mystr) - 4,
        "buffer size is not correct");

    cmpstr[4] = '\0';

    fail_unless(strncmp(cmpstr, "the ", sizeof(mystr)) == 0,
        "buffer read does not match expected string");

    len = octo_buffer_read(&buf, (uint8_t*)cmpstr, 6);

    fail_unless(octo_buffer_size(&buf) == sizeof(mystr) - 10,
        "buffer size is not correct");

    cmpstr[6] = '\0';

    fail_unless(strncmp(cmpstr, "world ", sizeof(mystr)) == 0,
        "buffer read does not match expected string");

    len = octo_buffer_read(&buf, (uint8_t*)cmpstr, sizeof(mystr));

    fail_unless(octo_buffer_size(&buf) == 0,
        "buffer size is not correct");

    fail_unless(len == sizeof(mystr) - 10,
        "buffer read did not return correct length");

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
