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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Buffer Item
 */

typedef struct octo_buffer_item octo_buffer_item;

struct octo_buffer_item
{
    bool free_data;
    size_t size;
    uint8_t *data;
    octo_list list;
};

typedef struct octo_buffer
{
    octo_list *buffer_list;
    size_t size;
    size_t items; 
} octo_buffer;

void octo_buffer_init(octo_buffer *b);
void octo_buffer_destroy(octo_buffer *b);

/**
 * information about the buffer
 */
size_t octo_buffer_size(const octo_buffer *b);
size_t octo_buffer_items(const octo_buffer *b);

/**
 * write to the buffer from some other location, copies only if explicitly told
 * to do so.
 *
 * return the actual number of bytes written.
 */
size_t octo_buffer_write(octo_buffer *b, uint8_t *data, size_t len, bool free_data);
size_t octo_buffer_write_copy(octo_buffer *b, uint8_t *data, size_t len);
size_t octo_buffer_write_file(octo_buffer *b, int fd, size_t len);

/**
 * read from the buffer to some other location, copies only if explicitly told
 * to do so.
 *
 * return the actual number of bytes read.
 */
size_t octo_buffer_read(octo_buffer *b, uint8_t *data, size_t len);
size_t octo_buffer_read_copy(octo_buffer *b, uint8_t *data, size_t len);
size_t octo_buffer_read_file(octo_buffer *b, int fd, size_t len);

/**
 * peek in to the buffer (like reading but does not remove bytes)
 *
 * return the actual number of bytes read.
 */
size_t octo_buffer_peek(octo_buffer *b, uint8_t *data, size_t len);
size_t octo_buffer_peek_copy(octo_buffer *b, uint8_t *data, size_t len);
