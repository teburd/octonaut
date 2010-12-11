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

/**
 * http request
 *
 * a http 1.1 request may contain several messages of varying sorts
 * so the request must be parsed in to individual messages to then be dealt
 * with
 */
struct octo_http_request
{
    /* http messages filled in as needed */
    octo_list message_queue;

    /* http parser and its state */
    int parser_state;
    http_parser parser;
};


/**
 * obtain a pointer to an uninitialized http request
 */
octo_http_request * octo_http_request_alloc(octo_http_request *request);

/**
 * release a pointer to a destroyed http request
 */
void octo_http_request_free(octo_http_request *request);

/**
 * initialize a http request
 */
void octo_http_request_init(octo_http_request *request);

/**
 * destroy a http request
 */
void octo_http_request_destroy(octo_http_request *request);

/**
 * create and initialize a http request
 */
octo_http_request * octo_http_request_new(octo_http_request *request);

/**
 * destroy and free a http request
 */
void octo_http_request_delete(octo_http_request *request);

/**
 * parse an incoming stream in to http_messages
 *
 * returns the number of new messages parsed
 */
size_t octo_http_request_parse(octo_http_request *request, const char *data, size_t len);

