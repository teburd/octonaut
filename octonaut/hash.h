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

#include "hash_function.h"

#include <stdbool.h>


/**
 * intrusive fixed size chained hash table store.
 *
 * choice of hash function is up to the creator of the store.
 * this takes care of retrieving, storing, and removing entries.
 *
 * it uses power of 2 sizes only to avoid using a true modulus
 * and asserts that the hash table really is power of 2.
 * modulus operations are very very slow, making hash bin lookups slow!
 *
 * This hash table is never resized to simplify things. Chaining
 * is used instead of open addressing. Perhaps an open addressing
 * version could be made pretty easily.
 *
 */

typedef struct octo_hash_entry octo_hash_entry;

/**
 * hash table
 */
typedef struct octo_hash
{
    octo_hash_function hash_function;
    uint32_t hash_seed;
    uint32_t n_hash_bins;
    size_t size;
    octo_hash_entry **hash_bins;
} octo_hash;

/**
 * intrusive entry in the hash table
 */
struct octo_hash_entry
{
    struct octo_hash_entry *next;
    size_t keylen;
    void *key;
};

/**
 * initialize a hash entry
 */
void octo_hash_entry_init(octo_hash_entry *entry, void *key,
        size_t keylen);

/**
 * initialize a hash entry using C99 struct initialization
 */
#define octo_hash_entry_sinit(key, keylen) { .next = NULL, .key = key, .keylen = keylen }

/**
 * initialize a hash table
 */
void octo_hash_init(octo_hash *hashtable,
        octo_hash_function hash_function, uint32_t seed, size_t pow2size);

/**
 * destroy a hash table
 */
void octo_hash_destroy(octo_hash *hashtable);

/**
 * size of a hash table
 */
size_t octo_hash_size(const octo_hash *hashtable);

/**
 * check if a hash table has a key
 */
bool octo_hash_has(octo_hash *hashtable, void *key, size_t keylen);

/**
 * put an entry in to the hash table
 *
 * returns true if put succeeded, false if they key already existed
 */
bool octo_hash_put(octo_hash *hashtable, octo_hash_entry *entry);

/**
 * get an entry from the hash table
 */
octo_hash_entry * octo_hash_get(octo_hash *hashtable,
        void *key, size_t keylen);

/**
 * get and remove an entry from the hash table
 */
octo_hash_entry * octo_hash_pop(octo_hash *hashtable,
        void *key, size_t keylen);

#endif
