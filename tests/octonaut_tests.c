#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ev.h>
#include <check.h>

#include "test_aio.c"

Suite* octonaut_suite(void)
{
    Suite *s = suite_create("octonaut");
    suite_add_tcase(s, octo_aio_tcase()); 
    return s;
}

int main(void)
{   
    int number_failed = 0;
    Suite* s = octonaut_suite();
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return 0;
}
