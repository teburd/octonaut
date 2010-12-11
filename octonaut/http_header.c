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

#include "http_header.h"

octo_http_header * octo_http_header_alloc()
{
    return malloc(sizeof(octo_http_header));
}

void octo_http_header_free(octo_http_header *header)
{
    free(header);
}

void octo_http_header_init(octo_http_header *header)
{
    octo_list_init(&header->header_list);
    octo_buffer_init(&header->field, 256);
    octo_buffer_init(&header->value, 256);
}

void octo_http_header_destroy(octo_http_header *header)
{
    octo_list_destroy(&header->header_list);
    octo_buffer_destroy(&header->field);
    octo_buffer_destroy(&header->value);
}

octo_http_header * octo_http_header_new()
{
    octo_http_header * header = octo_http_header_alloc();
    octo_http_header_init(header);
    return header;
}

void octo_http_header_delete(octo_http_header * header)
{
    octo_http_header_destroy(header);
    octo_http_header_free(header);
}


