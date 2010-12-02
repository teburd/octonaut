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


/**
 * intrusive hash table store.
 *
 * choice of hash function is up to the creator of the store
 * otherwise takes care of finding, storing, removing, and
 * to some degree iterating over the hash map.
 *
 * it uses power of 2 sizes only to avoid using a true modulus.
 * modulus operations are very very slow, making hash bin lookups
 * slow.
 *
 */

typedef uint64_t (*octo_hash_function)(uint8_t *key, size_t keylen);

typedef struct octo_hash
{
    uint64_t n_hash_bins;
    octo_list hash_bins[];
} octo_hash;

typedef struct octo_hash_entry
{
    octo_list hash_list;
    size_t keylen;
    uint8_t *key;
} octo_hash_entry;

static bool power_of_two(uint64_t x)
{
    return (x!=0) && ((x&(x-1)) == 0);
}

static inline void octo_hash_init(octo_hash *hash, octo_hash_function hash_function)
{
    assert(hash->n_hash_bins != 0);
    assert(power_of_two(hash->n_hash_bins));
}

static inline void octo_hash_destroy(octo_hash *hash)
{
}

/**
 * obtain the hash bin number given the number of bins and a hash
 */
static inline uint64_t octo_hash_nbin(uint64_t bins, uint64_t hash)
{
    return (hash & (bins-1));
}

/**
 * obtain the hash bin given the hash and key
 */
static inline octo_list * octo_hash_bin(octo_hash *hash, uint64_t hash)
{
    uint64_t nbin = octo_hash_nbin(hash->n_hash_bins, hash);
    return hash->hash_bins[nbin];
}

static inline uint64_t octo_hash_size(const octo_hash *hash)
{
    uint64_t size = 0;
    for(uint64_t i = 0; i < hash->n_hash_bins; ++i)
    {
        size += octo_list_size(hash->hash_bins[i]);
    }
    return size;
}


/**
 * put an entry in to the hash table
 */
void octo_hash_put(octo_hash *hash, octo_hash_entry *entry)
{
}

/**
 * get an entry from the hash table
 */
octo_hash_entry * octo_hash_get(octo_hash *hash, uint8_t *key, size_t keylen)
{
}

/**
 * get and remove an entry from the hash table
 */
octo_hash_entry * octo_hash_pop(octo_hash *hash, uint8_t *key, size_t keylen)
{
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
