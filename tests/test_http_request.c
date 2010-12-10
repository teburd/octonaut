#include <octohttp/http_request.h>

START_TEST (test_octo_http_request_init)
{


}
END_TEST

TCase* octo_http_request_tcase()
{
    TCase* tc_octo_http_request = tcase_create("octo_http_request");
    tcase_add_test(tc_octo_http_request, test_octo_http_request_init);
    return tc_octo_http_request;
}


