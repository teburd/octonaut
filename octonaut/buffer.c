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

#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 4096 

#define min(x, y) ((x)<(y)?(x):(y))
#define max(x, y) ((x)<(y)?(x):(y))

/**
 * alloc a buffer item from somewhere.
 *
 * In the future there might be a fixed stack pool of buffers (very likely)
 * such that stack buffers are used first until there are none free to use
 * after which malloc'd buffers are allocated and freed as necessary.
 *
 * Keeping a set of stack buffers may substantially speed up buffering
 * as no malloc/free calls would be needed.
 *
 * That is the reasoning for the api.
 *
 * notably the requested size may be different than the returned size.
 */
static inline octo_buffer_item * octo_buffer_item_alloc(size_t len)
{
    octo_buffer_item *item;

    item = malloc(sizeof(octo_buffer_item) + len);
    item->start = 0;
    item->size = 0;
    item->capacity = len;

    return item;
}

/**
 * free a buffer item to somewhere, possibly returning it
 * to a list containing a free buffer pool (stack buffer pool)
 * or actually calling free()
 */
static inline void octo_buffer_item_free(octo_buffer_item *item)
{
    octo_list_remove(&item->list);
    free(item);
}

/**
 * item bytes used
 */
static inline size_t octo_buffer_item_size(octo_buffer_item *item)
{
    return item->size;
}

/**
 * item bytes capacity
 */
static inline size_t octo_buffer_item_capacity(octo_buffer_item *item)
{
    return item->capacity;
}

/**
 * item bytes remaining to write to
 */
static inline size_t octo_buffer_item_remaining(octo_buffer_item *item)
{
    return item->capacity - (item->size + item->start);
}

void octo_buffer_init(octo_buffer *b)
{
    octo_list_init(&b->buffer_list);
    b->size = 0;
}

void octo_buffer_destroy(octo_buffer *b)
{
    octo_buffer_item *pos;
    octo_buffer_item *next;

    octo_list_foreach(pos, next, &b->buffer_list, list)
    {
        octo_buffer_item_free(pos);
    }

    octo_list_destroy(&b->buffer_list);
    b->size = 0;
}

size_t octo_buffer_size(const octo_buffer *b)
{
    return b->size;
}

size_t octo_buffer_write(octo_buffer *b, uint8_t *data, size_t len)
{
    size_t copylen = 0;
    size_t copied = 0;
    octo_buffer_item *item = NULL;
    octo_list *head = octo_list_head(&b->buffer_list);

    if(head != &b->buffer_list)
    {
        item = octo_list_entry(head, octo_buffer_item, list);
    }
    else
    {
        item = octo_buffer_item_alloc(CHUNK_SIZE);
        octo_list_push(&b->buffer_list, &item->list);
    }

    while(copied < len)
    {
        copylen = min(len-copied, octo_buffer_item_remaining(item));

        if(copylen > 0)
        {
            memcpy(&item->data[item->start+item->size], &data[copied], copylen);
            item->size += copylen;
            copied += copylen;
        }

        if(copied < len)
        {
            item = octo_buffer_item_alloc(CHUNK_SIZE);
            octo_list_push(&b->buffer_list, &item->list);
        }
    }

    b->size += copied;
    return copied;
}

size_t octo_buffer_read(octo_buffer *b, uint8_t *data, size_t len)
{
    size_t copied = 0;
    size_t copylen = 0;

    while(copied < len)
    {
        octo_list *tail = octo_list_tail(&b->buffer_list);

        if(tail == &b->buffer_list)
        {
            break;
        }

        octo_buffer_item *item = octo_list_entry(tail, octo_buffer_item, list);
        copylen = min(len-copied, octo_buffer_item_size(item));
        memcpy(&data[copied], &item->data[item->start], copylen);
        copied += copylen;

        item->start += copylen;
        item->size -= copylen;

        if(octo_buffer_item_size(item) == 0)
        {
            octo_buffer_item_free(item);
        }
    }

    b->size -= copied;
    return copied;
}

size_t octo_buffer_peek(octo_buffer *b, uint8_t *data, size_t len)
{
    size_t copied = 0;
    size_t copylen = 0;

    octo_list *tail = octo_list_tail(&b->buffer_list);
    octo_buffer_item *item = octo_list_entry(tail, octo_buffer_item, list);

    if(tail == &b->buffer_list)
    {
        return 0;
    }

    while(copied < len)
    {
        copylen = min(len-copied, octo_buffer_item_size(item));
        memcpy(&data[copied], &item->data[item->start], copylen);
        copied += copylen;

        if(copied >= octo_buffer_item_size(item))
        {
            tail = tail->prev;
            item = octo_list_entry(tail, octo_buffer_item, list);
        }
    }

    b->size -= copied;
    return copied;

}
