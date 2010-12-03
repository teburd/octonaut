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

#include <assert.h>

#include "list.h"

#define min(x, y) ((x)<(y)?(x):(y))

/**
 * intrusive hash table store.
 *
 * choice of hash function is up to the creator of the store.
 * this takes care of retrieving, storing, and removing entries.
 *
 * it uses power of 2 sizes only to avoid using a true modulus
 * and asserts that the hash table really is power of 2.
 * modulus operations are very very slow, making hash bin lookups slow!
 *
 */

typedef uint32_t (*octo_hash_function)(const uint8_t *key, const size_t keylen, const uint32_t seed);

typedef struct octo_hash
{
    octo_hash_function hash_function;
    uint32_t hash_seed;
    uint32_t n_hash_bins;
    octo_list hash_bins[];
} octo_hash;

typedef struct octo_hash_entry
{
    octo_list hash_list;
    size_t keylen;
    uint8_t *key;
} octo_hash_entry;

static bool power_of_two(uint32_t x)
{
    return (x!=0) && ((x&(x-1)) == 0);
}

/**
 * initialize the hash table, does not allocate any memory
 */
static inline void octo_hash_init(octo_hash *hashtable, octo_hash_function hash_function, uint32_t seed)
{
    assert(hashtable->n_hash_bins != 0);
    assert(power_of_two(hashtable->n_hash_bins));
    hashtable->hash_function = hash_function;
    hashtable->hash_seed = seed;
}

/**
 * clear out the hash table of all entries, does not free any memory
 */
static inline void octo_hash_destroy(octo_hash *hashtable)
{
}

/**
 * obtain the hash bin number given the number of bins and a hash
 */
static inline uint32_t octo_hash_nbin(uint32_t bins, uint32_t keyhash)
{
    return (keyhash & (bins-1));
}

/**
 * obtain the hash bin given the hash and key
 */
static inline octo_list * octo_hash_bin(octo_hash *hashtable, uint32_t keyhash)
{
    uint32_t nbin = octo_hash_nbin(hashtable->n_hash_bins, keyhash);
    return &hashtable->hash_bins[nbin];
}

static inline size_t octo_hash_size(const octo_hash *hashtable)
{
    size_t size = 0;
    for(uint32_t i = 0; i < hashtable->n_hash_bins; ++i)
    {
        size += octo_list_size(&hashtable->hash_bins[i]);
    }
    return size;
}

/**
 * check if a bin has an entry
 */
static inline bool octo_hash_bin_has(octo_hash *hashtable, uint32_t keyhash, uint8_t *key, size_t keylen)
{
    octo_list *list = octo_hash_bin(hashtable, keyhash);
    octo_hash_entry *pos;
    octo_hash_entry *next;
    octo_list_foreach(pos, next, list, hash_list)
    {
        if(strncmp((char*)key, (char*)pos->key, min(pos->keylen, keylen)) == 0)
        {
            return true;
        }
    }

    return false;
}

/**
 * has an entry
 */
static inline bool octo_hash_has(octo_hash *hashtable, uint8_t *key, size_t keylen)
{
    uint32_t keyhash = hashtable->hash_function(key, keylen, hashtable->hash_seed);
    return octo_hash_bin_has(hashtable, keyhash, key, keylen);
}

/**
 * put an entry in to the hash table, duplicate keys are not put in, this is
 * not a multi hash!
 */
static inline void octo_hash_put(octo_hash *hashtable, octo_hash_entry *entry)
{
    uint32_t keyhash = hashtable->hash_function(entry->key, entry->keylen, hashtable->hash_seed);
    octo_list *list = octo_hash_bin(hashtable->n_hash_bins, keyhash);

    if(octo_list_empty(list))
    {
        octo_list_add(list, &entry->hash_list);
    }
    else if(!octo_hash_bin_has(hashtable, keyhash, entry->key, entry->keylen))
    {
        octo_list_add(list, &entry->hash_list);
    }
}

/**
 * get an entry from the hash table
 */
octo_hash_entry * octo_hash_get(octo_hash *hashtable, uint8_t *key, size_t keylen)
{
    uint32_t keyhash = hashtable->hash_function(key, keylen, hashtable->hash_seed);
    octo_list *list = octo_hash_bin(hashtable->n_hash_bins, keyhash);
    octo_hash_entry *pos;
    octo_hash_entry *next;
    octo_list_foreach(pos, next, list, hash_list)
    {
        if(strncmp((char*)key, (char*)pos->key, min(pos->keylen, keylen)) == 0)
        {
            return pos;
        }
    }
    return NULL;
}

/**
 * get and remove an entry from the hash table
 */
octo_hash_entry * octo_hash_pop(octo_hash *hashtable, uint8_t *key, size_t keylen)
{
    uint32_t keyhash = hashtable->hash_function(key, keylen, hashtable->hash_seed);
}

/**
 * The clever little macro that could. Obtains a pointer to the struct
 * that the octo_hash_entry is contained in. This is done by subtracting the offset
 * of the octo_hash_entry member from the address of the address of the octo_hash_entry
 * member.
 */
#define octo_hash_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif
