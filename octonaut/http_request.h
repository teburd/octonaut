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


#include <netinet/in.h>

#include "list.h"
#include "hash.h"

#include "http_parser.h"

typedef struct octo_http_request octo_http_request;

/**
 * http key/value header pair
 */
typedef struct octo_http_header
{
    octo_hash_entry header_hash;
    octo_list header_list;
    char *field;
    size_t field_len;
    char *value;
    size_t value_len;
} octo_http_header;

/**
 * enum used to track what the callbacks should do
 */

/**
 * http request struct that can be 
 * feed incrementally with data.
 */
struct octo_http_request
{
    /* generic stores for headers */
    octo_list header_list;
    octo_hash header_hash;

    /* direct access to common header information */
    char *url;

    /* http parser and its state */
    int parser_state;
    http_parser parser;
};

void octo_http_request_init(octo_http_request *request);
void octo_http_request_destroy(octo_http_request *request);
bool octo_http_request_parse(octo_http_request *request, const char *data, size_t len);

