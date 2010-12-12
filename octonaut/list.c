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
#include <stdint.h>

#include "list.h"

inline void octo_list_init(octo_list *l)
{
    l->next = l;
    l->prev = l;
}

inline void octo_list_destroy(octo_list *l)
{
    octo_list *pos = l->next;
    octo_list *cur = l->next;

    while(pos != l)
    {
        cur = pos;
        pos = cur->next;
        octo_list_init(cur);
    }

    octo_list_init(l);
}

/**
 * insert a new octo_list * in between the prev and next octo_list *
 */
inline void _octo_list_insert(octo_list *new, octo_list *prev,
    octo_list *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * append an item to the list right after the given one
 */
static inline void _octo_list_append(octo_list *list, octo_list *item)
{
    _octo_list_insert(item, list, list->next);
}

/**
 * prepend an item to the list right before the given one
 */
static inline void _octo_list_prepend(octo_list *list, octo_list *item)
{
    _octo_list_insert(item, list->prev, list);
}

inline void octo_list_add(octo_list *list, octo_list *item)
{
    _octo_list_append(list, item);
}

inline void octo_list_prepend(octo_list *list, octo_list *item)
{
    _octo_list_append(list, item);
}

inline void octo_list_append(octo_list *list, octo_list *item)
{
    _octo_list_prepend(list, item);
}

inline void octo_list_remove(octo_list *item)
{
    octo_list *next = item->next;
    octo_list *prev = item->prev;
    prev->next = next;
    next->prev = prev;
    item->next = item;
    item->prev = item;
}

inline void octo_list_push(octo_list *list, octo_list *item)
{
    octo_list_add(list, item);
}

inline octo_list * octo_list_pop(octo_list *list)
{
    octo_list *item = list->prev;
    octo_list_remove(item);
    return item;
}

inline octo_list * octo_list_tail(octo_list *list)
{
    return list->prev;
}

inline octo_list * octo_list_head(octo_list *list)
{
    return list->next;
}

inline bool octo_list_empty(octo_list *list)
{
    if(list->next == list && list->prev == list)
    {
        return true;
    }
    return false;
}

/**
 * obtain the size of the list
 */
inline size_t octo_list_size(const octo_list *list)
{
    size_t count = 0;
    octo_list *pos = list->next;
    octo_list *cur = list->next;

    while(pos != list)
    {
        cur = pos;
        pos = cur->next;
        count += 1;
    }

    return count;
}
