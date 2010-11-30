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
 * A queued buffer with a static pool of buffers to use.
 *
 * Should require very few malloc/free calls!
 */

typedef struct octo_buffer_item
{
    octo_list list;
    size_t start;
    size_t size;
    size_t capacity;
    uint8_t data[];
} octo_buffer_item;

typedef struct octo_buffer
{
    octo_list buffer_list;
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
void octo_buffer_init(octo_buffer *b);

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
size_t octo_buffer_items(const octo_buffer *b);

/**
 * write to the buffer from a memory location at most len bytes
 *
 * return number of bytes written
 */
size_t octo_buffer_write(octo_buffer *b, uint8_t *data, size_t len);

/**
 * write to the buffer from a file descriptor at most len bytes
 *
 * return number of bytes written
 */
size_t octo_buffer_fwrite(octo_buffer *b, int fd, size_t len);

/**
 * read from the buffer to a memory location at most len bytes.
 *
 * return the number of bytes read.
 */
size_t octo_buffer_read(octo_buffer *b, uint8_t *data, size_t len);

/**
 * read from the buffer to a file descriptor at most len bytes.
 *
 * return the number of bytes read.
 */
size_t octo_buffer_fread(octo_buffer *b, int fd, size_t len);

/**
 * peek in to the buffer at most len bytes.
 *
 * same as octo_buffer_read except this does not remove bytes
 * from the buffer.
 *
 * return the actual number of bytes read.
 */
size_t octo_buffer_peek(const octo_buffer *b, uint8_t *data, size_t len);

/**
 * remove from the buffer at most len bytes.
 *
 * return the actual number of bytes removed.
 */
size_t octo_buffer_drain(octo_buffer *b, size_t len);

#endif
