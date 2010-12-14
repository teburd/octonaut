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

#include <octonaut/http_message.h>
#include <octonaut/http_header.h>
#include <check.h>

typedef struct test_header
{
    char *field;
    char *value;
} test_header;

static test_header test_headers[10] = {
    {"Content-Length", "100"},
    {"Content-Type", "text/event-stream"},
    {"Cache-Control", "no-cache"},
    {"Accept", "text/plain"},
    {"Accept-Encoding", "gzip"},
    {"Accept-Language", "en"},
    {"Allow", "GET, HEAD, PUT, POST"},
    {"Connection", "close"},
    {"From", "test@octonaut.com"},
    {"Host", "test.octonaut.com"}
};

START_TEST (test_octo_http_message_new_delete)
{
    octo_http_message *message = octo_http_message_new();
    octo_http_message_delete(message);
}
END_TEST

START_TEST (test_octo_http_message_add_headers)
{
    octo_http_message *message = octo_http_message_new();
    octo_http_header *headers[10];

    for(int i = 0; i < 10; ++i)
    {
        headers[i] = octo_http_header_new();
        octo_buffer_write(&headers[i]->field, test_headers[i].field,
            strlen(test_headers[i].field));
        octo_buffer_write(&headers[i]->value, test_headers[i].value,
            strlen(test_headers[i].value));

        octo_http_message_add_header(message, headers[i]);
    }

    for(int i = 0; i < 10; ++i)
    {
        octo_http_header_delete(headers[i]);
    }

    octo_http_message_delete(message);
}
END_TEST


TCase* octo_http_message_tcase()
{
    TCase* tc_octo_http_message = tcase_create("octo_http_message");
    tcase_add_test(tc_octo_http_message, test_octo_http_message_new_delete);
    tcase_add_test(tc_octo_http_message, test_octo_http_message_add_headers);
    return tc_octo_http_message;
}


