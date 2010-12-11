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

#ifndef OCTO_BUFFER_H
#define OCTO_BUFFER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "list.h"

/**
 * fast read/write buffer for network IO
 */
typedef struct octo_buffer_chunk
{
    octo_list list;
    size_t start;
    size_t size;
    size_t capacity;
    uint8_t data[];
} octo_buffer_chunk;

typedef struct octo_buffer
{
    octo_list buffer_list;
    size_t chunk_size;
    size_t size;
    size_t items; 
} octo_buffer;

/**
 * initialize the stack buffers
 */
void octo_init_buffer();

/**
 * initialize a buffer
 */
void octo_buffer_init(octo_buffer *b, size_t chunk_size);

/**
 * destroy a buffer
 */
void octo_buffer_destroy(octo_buffer *b);

/**
 * size of the buffer in bytes
 */
size_t octo_buffer_size(const octo_buffer *b);

/**
 * number of elements in the buffer
 */
size_t octo_buffer_chunks(const octo_buffer *b);

/**
 * write to the buffer from a memory location at most len bytes
 *
 * return number of bytes written
 */
size_t octo_buffer_write(octo_buffer *b, void *data, size_t len);

/**
 * read from the buffer to a memory location at most len bytes.
 *
 * return the number of bytes read.
 */
size_t octo_buffer_read(octo_buffer *b, void *data, size_t len);

/**
 * peek in to the buffer at most len bytes.
 *
 * same as octo_buffer_read except this does not remove bytes
 * from the buffer.
 *
 * return the actual number of bytes read.
 */
size_t octo_buffer_peek(octo_buffer *b, void *data, size_t len);

/**
 * remove from the buffer at most len bytes.
 *
 * return the actual number of bytes removed.
 */
size_t octo_buffer_drain(octo_buffer *b, size_t len);

#endif
