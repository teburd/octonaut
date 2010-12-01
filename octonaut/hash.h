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

#ifndef OCTO_HASH_H
#define OCTO_HASH_H

#include "list.h"

/**
 * fast, simple, intrusive hash map store.
 */
typedef struct octo_hash
{
    size_t n_hash_bins;
    octo_list hash_bins[];
} octo_hash;

typedef struct octo_hash_item
{
    uint32_t hash;
    octo_list *hash_list;
    octo_hash *hash_table;
} octo_hash_item;

void octo_hash_init(octo_hash *hash);

void octo_hash_add(octo_hash *hash, octo_hash_item *item);

void octo_hash_remove(octo_hash_item *item);

/**
 * The clever little macro that could. Obtains a pointer to the struct
 * that the octo_list is contained in. This is done by subtracting the offset
 * of the octo_list member from the address of the address of the octo_list 
 * member.
 */
#define octo_hash_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
