/**
 * Copyright (c) 2010 Tom Burdick <thomas.burdick@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <octonaut/http_request.h>
#include <check.h>

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

START_TEST (test_octo_http_request_partial_message)
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

START_TEST (test_octo_http_request_complete_message)
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

TCase* octo_http_request_tcase()
{
    TCase* tc_octo_http_request = tcase_create("octo_http_request");
    tcase_add_test(tc_octo_http_request, test_octo_http_request_init);
    tcase_add_test(tc_octo_http_request, test_octo_http_request_partial_message);
    tcase_add_test(tc_octo_http_request, test_octo_http_request_complete_message);
    return tc_octo_http_request;
}


