#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ev.h>
#include <check.h>

#include "common.h"

#include "test_list.c"
#include "test_buffer.c"
#include "test_hash_function.c"
#include "test_hash.c"
#include "test_logger.c"
#include "test_aio.c"
#include "test_server.c"
#include "test_http_request.c"

Suite* octonaut_suite(void)
{
    Suite *s = suite_create("octonaut");
    suite_add_tcase(s, octo_list_tcase());
    suite_add_tcase(s, octo_buffer_tcase()); 
    suite_add_tcase(s, octo_hash_function_tcase()); 
    suite_add_tcase(s, octo_hash_tcase()); 
    suite_add_tcase(s, octo_logger_tcase()); 
    suite_add_tcase(s, octo_aio_tcase()); 
    suite_add_tcase(s, octo_server_tcase()); 
    suite_add_tcase(s, octo_http_request_tcase()); 
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
