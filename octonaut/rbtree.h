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

#ifndef OCTO_RBTREE_H
#define OCTO_RBTREE_H

/**
 * intrusive red-black balanced binary tree
 */

typedef bool (* octo_rbtree_eq) (octo_rbtree_entry *lh, octo_rbtree_entry *rh);
typedef bool (* octo_rbtree_lt) (octo_rbtree_entry *lh, octo_rbtree_entry *rh);

/**
 * struct used for an entry
 */
typedef struct octo_rbtree_entry
{
    octo_rbtree_entry *parent;
    octo_rbtree_entry *lchild;
    octo_rbtree_entry *rchild;
} octo_rbtree entry;

/**
 * struct used for searching
 */
typedef struct octo_rbtree_s_entry
{
    bool (* s_entry_eq)(octo_rbtree_s_entry *lh, octo_rbtree_entry *rh);
    bool (* s_entry_lt)(octo_rbtree_s_entry *lh, octo_rbtree_entry *rh);
} octo_rbtree_s_entry;

/**
 * struct for tree management
 */
typedef struct octo_rbtree
{
    octo_rbtree_eq eq_function;
    octo_rbtree_lt lt_function;
    octo_rbtree_entry root_entry;
} octo_rbtree;

typedef struct octo_rbtree_iterator
{
    octo_rbtree_entry *current;
    octo_rbtree_entry *next;
} octo_rbtree_iterator;

void octo_rbtree_init(octo_rbtree *tree, octo_rbtree_eq eq, octo_rbtree_lt lt);
void octo_rbtree_destroy(octo_rbtree *tree);
void octo_rbtree_insert(octo_rbtree *tree, octo_rbtree_entry *entry);
void octo_rbtree_remove(octo_rbtree_entry *entry);
octo_rbtree_entry * octo_rbtree_find(octo_rbtree *tree, octo_rbtree_entry *entry);

#endif
