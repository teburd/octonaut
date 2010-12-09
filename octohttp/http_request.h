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


#include <octonaut/hash.h>
#include <sys/types.h>

/**
 * callback performed when request is ready
 */
typedef void (* octo_http_request_cb) (octo_http_request *request);


/**
 * http key/value header pair
 */
typedef struct octo_http_header
{
    octo_hash_entry hash_entry;
    char *field;
    size_t field_len;
    char *value;
    size_t value_len;
} octo_http_header;

/**
 * http request handler with buffered asynchronous IO
 */
typedef struct octo_http_request
{
    octo_aio aio;
    octo_hash headers;
    octo_buffer body;
    ev_timer timer;
    struct timeval activity;
    struct timeval touch;
    http_parser parser;
    octo_http_request_cb request_cb;
} octo_http_request;

/**
 * handle a http request, automatically allocates things as needed.
 * in general it will use some process memory (the memory allocated for
 * static and global variables) before ever touching malloc/free.
 */
void octo_http_request(int fd, struct sockaddr_in *addr, octo_http_request_cb request_cb);
