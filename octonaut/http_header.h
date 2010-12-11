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

#ifndef OCTO_HTTP_HEADER_H
#define OCTO_HTTP_HEADER_H

/**
 * http key/value header pair
 */
typedef struct octo_http_header
{
    octo_hash_entry header_hash;
    octo_list header_list;
    octo_buffer field;
    octo_buffer value;
} octo_http_header;

/**
 * obtain a pointer to an uninitialized http header
 */
void octo_http_header_alloc(octo_http_header *header);

/**
 * release a pointer to an uninitialized http header
 */
void octo_http_header_free(octo_http_header *header);

/**
 * initialize http header
 */
void octo_http_header_init(octo_http_header *header);

/**
 * destroy http header
 */
void octo_http_header_destroy(octo_http_header *header);

/**
 * append to the field of the header
 */
void octo_http_header_field_append(octo_http_header *header, uint8_t *field, size_t len);

/**
 * append to the value of the header
 */
void octo_http_header_value_append(octo_http_header *header, uint8_t *field, size_t len);

#endif
