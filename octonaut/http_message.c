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

#include "http_message.h"

void octo_http_message_init(octo_http_message *message)
{
    octo_list_init(&message->message_queue);
    message->method = -1;

    octo_buffer_init(&message->path, 256);
    octo_buffer_init(&message->query, 256);
    message->http_major_version = 0;
    message->http_minor_version = 0;
    octo_hash_init(&message->headers, octo_default_hash_function, rand(), 64);
    octo_buffer_init(&message->body, 1024);
}

void octo_http_message_destroy(octo_http_message *message)
{
    octo_list_destroy(&message->message_queue);
    message->method = -1;

    octo_buffer_destroy(&message->path);
    octo_buffer_destroy(&message->query);
    message->http_major_version = 0;
    message->http_minor_version = 0;

    /*
    octo_hash_iterator iter;
    octo_http_header *header = octo_hash_iterator_init(&message->headers, &iter);
    do {
        octo_http_header_free(header);
    } while(header = octo_hash_iterator_next(iter, octo_http_header, header_hash))
    */

    octo_hash_destroy(&message->headers);

    octo_buffer_destroy(&message->body);
}
