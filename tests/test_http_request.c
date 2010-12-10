#include <octohttp/http_request.h>

START_TEST (test_octo_http_request_init)
{
    const char *request_str = "GET / HTTP/1.1\r\n"
                               "Host: 0.0.0.0:5000\r\n"
                               "Bogus: mogus\r\n"
                               "\r\n";
    octo_http_request request;
    octo_http_request_init(&request);
    
    octo_http_request_parse(&request, request_str, strlen(request_str));

    octo_http_request_destroy(&request);
}
END_TEST

TCase* octo_http_request_tcase()
{
    TCase* tc_octo_http_request = tcase_create("octo_http_request");
    tcase_add_test(tc_octo_http_request, test_octo_http_request_init);
    return tc_octo_http_request;
}


