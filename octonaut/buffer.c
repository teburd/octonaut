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

#include "buffer.h"

octo_list free_buffers;

void octo_buffer_init(octo_buffer *b)
{
    octo_list_init(&b->buffer_list);
    b->size = 0;
}

void octo_buffer_destroy(octo_buffer *b)
{
    octo_list_destroy(&b->buffer_list);
    b->size = 0;
}

size_t octo_buffer_size(const octo_buffer *b)
{
    return b->size;
}

size_t octo_buffer_write(octo_buffer *b, uint8_t *data, size_t len)
{
    octo_buffer_item *item;

    if(!octo_list_empty(free_buffers))
    {
       item = octo_list_pop(free_buffers); 
    }
    else
    {
        item = malloc(sizeof(octo_buffer_item) + len);
    }

    item->size = len;
    item->start = 0;
    item->end = len-1;
    memcpy(item->data, data, len);
    octo_list_push(b->buffer_list, item->list);
    return len;
}

size_t octo_buffer_read(octo_buffer *b, uint8_t *data, size_t len)
{
    octo_buffer_item *item = octo_list_entry(octo_list_tail(b->buffer_list), octo_buffer_item, list);
    memcpydd
}
