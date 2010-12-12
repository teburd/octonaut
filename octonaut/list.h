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

#ifndef OCTO_LIST_H
#define OCTO_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "common.h"

/**
 * intrusive doubly linked list much like the linux kernel's linked list.
 *
 * The trick is using the typeof() and offsetof() macros to avoid passing
 * around void * or having to do a bunch of type specific functions.
 *
 */

/**
 * doubly linked list
 */
typedef struct octo_list {
    struct octo_list *next;
    struct octo_list *prev;
} octo_list;

/**
 * initialize octo_list prev/next pointers
 */
void octo_list_init(octo_list *l);

/**
 * destroy the octo_list
 *
 * really just reinitializes the list
 */
void octo_list_destroy(octo_list *l);

/**
 * add an item to the list in no particular way
 */
void octo_list_add(octo_list *list, octo_list *item);

/**
 * prepend an item to the head of the list
 */
void octo_list_prepend(octo_list *list, octo_list *item);

/**
 * append an item to the tail of the list
 */
void octo_list_append(octo_list *list, octo_list *item);

/**
 * remove an item from the list
 */
void octo_list_remove(octo_list *item);

/**
 * push an item on to the head of the list
 */
void octo_list_push(octo_list *list, octo_list *item);

/**
 * pop an item off the tail of the list
 *
 * may return the list itself in the case where the list is empty.
 */
octo_list * octo_list_pop(octo_list *list);

/**
 * the tail of the list
 *
 * may return the list itself in the case where the list is empty.
 */
octo_list * octo_list_tail(octo_list *list);

/**
 * the head of the list
 *
 * may return the list itself in the case where the list is empty.
 */
octo_list * octo_list_head(octo_list *list);

/**
 * test if the list is empty 
 */
bool octo_list_empty(octo_list *list);

/**
 * obtain the size of the list
 */
size_t octo_list_size(const octo_list *list);

/**
 * iterate over each item in the list, you can manipulate this current item
 * but not the next item in the list however you like including deleting it.
 */
#define octo_list_foreach(pos, n, list, member) \
    for (pos = ptr_offset((list)->next, typeof(*pos), member), \
        n = ptr_offset(pos->member.next, typeof(*pos), member); \
        &pos->member != (list); \
        pos = n, n = ptr_offset(n->member.next, typeof(*n), member))

#endif
