#include <octonaut/http_request.h>

START_TEST (test_octo_http_request_init)
{
    const char request_str[] = "GET / HTTP/1.1\r\n"
                               "Host: 0.0.0.0:5000\r\n"
                               "Bogus: mogus\r\n"
                               "\r\n";
    octo_http_request request;
    octo_http_request_init(&request);

    octo_http_request_parse(&request, request_str, strlen(request_str));

    octo_http_request_destroy(&request);
}
END_TEST

START_TEST (test_octo_http_request_hash)
{
    const char request_str[] = "GET / HTTP/1.1\r\n"
                               "Host: 0.0.0.0:5000\r\n"
                               "Bogus: mogus\r\n"
                               "\r\n";
    octo_http_request request;
    octo_http_request_init(&request);

    octo_http_request_parse(&request, request_str, strlen(request_str));

    fail_unless(octo_hash_has(&request.header_hash, "Host", 4),
        "hash table doesn't have the Host key");

    octo_http_request_destroy(&request);
}
END_TEST

START_TEST (test_octo_http_request_list)
{
    const char request_str[] = "GET / HTTP/1.1\r\n"
                               "Host: 0.0.0.0:5000\r\n"
                               "Bogus: mogus\r\n"
                               "\r\n";
    octo_http_request request;
    octo_http_request_init(&request);

    octo_http_request_parse(&request, request_str, strlen(request_str));

    fail_unless(octo_hash_has(&request.header_hash, "Host", 4),
        "hash table doesn't have the Host key");

    octo_http_request_destroy(&request);
}
END_TEST

TCase* octo_http_request_tcase()
{
    TCase* tc_octo_http_request = tcase_create("octo_http_request");
    tcase_add_test(tc_octo_http_request, test_octo_http_request_init);
    tcase_add_test(tc_octo_http_request, test_octo_http_request_hash);
    tcase_add_test(tc_octo_http_request, test_octo_http_request_list);
    return tc_octo_http_request;
}


