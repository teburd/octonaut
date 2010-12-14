#include <check.h>

#include "aio.h"
#include "list.h"
#include "buffer.h"
#include "hash_function.h"
#include "hash.h"
#include "logger.h"
#include "server.h"
#include "http_header.h"

/*
#include "http_message.h"
#include "http_request.h"
#include "http_server.h"
*/


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
    suite_add_tcase(s, octo_http_header_tcase());
    suite_add_tcase(s, octo_http_message_tcase());
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
