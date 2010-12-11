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

#ifndef OCTO_HTTP_MESSAGE_H
#define OCTO_HTTP_MESSAGE_H

#include "list.h"
#include "buffer.h"
#include "hash.h"
#include "http_header.h"
#include "http_parser.h"

typedef enum http_method octo_http_method;

/**
 * http message 
 *
 * method, url, version, set of key/value headers, and message body
 */
typedef struct octo_http_message
{
    octo_list message_queue;

    octo_http_method method; 
    octo_buffer path;
    octo_buffer query;
    int http_major_version;
    int http_minor_version;
    octo_hash headers;
    octo_buffer body;

} octo_http_message;

/**
 * initialize a http message
 */
void octo_http_message_init(octo_http_message *message);

/**
 * destroy a http message
 */
void octo_http_message_destroy(octo_http_message *message);

/**
 * allocate an uninitialized http message
 */
octo_http_message * octo_http_message_alloc();

/**
 * free a destroyed http message
 */
void octo_http_message_free(octo_http_message *message);

/**
 * allocate and initialize a http message 
 */
octo_http_message * octo_http_message_new();

/**
 * destroy and free a http message
 */
void octo_http_message_delete(octo_http_message *message);

/**
 * add a header to an http message
 */
void octo_http_message_add_header(octo_http_message *message,
        octo_http_header *header);

/**
 * pointer to the last header
 *
 * returns null if there are no headers
 */
octo_http_header * octo_http_message_last_header(octo_http_message *message);

#endif

