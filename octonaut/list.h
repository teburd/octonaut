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

/**
 * doubly linked intrusive list much like the linux kernel's linked list.
 *
 * The trick is using the typeof() macro to avoid passing in a bunch of
 * information about where in the struct the intrusive list is held.
 *
 * I avoid copying list.h from the kernel, but it does
 * not attempt to avoid taking most of the good ideas!
 */
typedef struct octo_list octo_list;

struct octo_list {
    octo_list *next;
    octo_list *prev;
};

/**
 * initialize octo_list prev/next pointers
 */
static inline void octo_list_init(octo_list *l, octo_list *n, octo_list *p)
{
    l->next = n;
    l->prev = p;
}

/**
 * insert a new octo_list * in between the prev and next octo_list *
 */
static inline void octo_list_insert(octo_list *new, octo_list *prev,
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
static inline void octo_list_append(octo_list *new, octo_list *existing)
{
    octo_list_insert(new, existing, existing->next);
}

/**
 * prepend an item to the list right before the given one
 */
static inline void octo_list_prepend(octo_list *new, octo_list *existing)
{
    octo_list_insert(new, existing->prev, existing);
}

/**
 * The clever little macro that could. Obtains a pointer to the struct
 * that the octo_list is contained in. This is done by subtracting the offset
 * of the octo_list member from the address of the octo_list memory address.
 */
#define octo_list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * iterate over each item in the list, you can manipulate this current item
 * but not the next item in the list however you like including deleting it.
 */
#define octo_list_foreach(pos, n, list, member) \
    for (pos = octo_list_entry((list)->next, typeof(*pos), member), \
        n = octo_list_entry(pos->member.next, typeof(*pos), member); \
        &pos->member != (list); \
        pos = n, n = octo_list_entry(n->member.next, typeof(*n), member))

#endif
