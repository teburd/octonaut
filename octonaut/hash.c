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
#include <memory.h>
#include <assert.h>

#include "common.h"

#include "hash.h"

/**
 * obtain the hash bin number given the number of bins and a hash
 */
static inline uint32_t octo_hash_nbin(uint32_t bins, uint32_t keyhash)
{
    return (keyhash & (bins-1));
}

/**
 * find an entry in a bin or return NULL
 */
static inline octo_hash_entry * octo_hash_bin_get(octo_hash_entry *entry, void *key, size_t keylen)
{
    while(entry != NULL)
    {
        if(keylen != entry->keylen)
        {
            continue;
        }
        
        if(memcmp(entry->key, key, keylen) == 0)
        {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

inline void octo_hash_entry_init(octo_hash_entry *entry, void *key,
        size_t keylen)
{
    entry->key = key;
    entry->keylen = keylen;
    entry->next = NULL;
}

inline void octo_hash_init(octo_hash *hashtable, octo_hash_function hash_function, uint32_t seed, size_t pow2size)
{
    assert(pow2size < 32);
    hashtable->hash_function = hash_function;
    hashtable->hash_seed = seed;
    hashtable->n_hash_bins = (1<<pow2size);
    hashtable->hash_bins = malloc(sizeof(octo_hash_entry *)*hashtable->n_hash_bins);

    for(size_t i = 0; i < hashtable->n_hash_bins; ++i)
    {
        hashtable->hash_bins[i] = NULL;
    }

    assert(hashtable->n_hash_bins != 0);
    assert(power_of_two(hashtable->n_hash_bins));
}

inline void octo_hash_destroy(octo_hash *hashtable)
{
    hashtable->hash_function = NULL;
    hashtable->hash_seed = 0;
    hashtable->n_hash_bins = 0;
    free(hashtable->hash_bins);
    hashtable->hash_bins = NULL;
}

inline size_t octo_hash_size(const octo_hash *hashtable)
{
    return hashtable->size;
}

inline bool octo_hash_has(octo_hash *hashtable, void *key, size_t keylen)
{
    uint32_t keyhash = hashtable->hash_function(key, keylen, hashtable->hash_seed);
    uint32_t nbin = octo_hash_nbin(hashtable->n_hash_bins, keyhash);
    octo_hash_entry *entry = hashtable->hash_bins[nbin];
    return (bool)octo_hash_bin_get(entry, key, keylen);
}

inline bool octo_hash_put(octo_hash *hashtable, octo_hash_entry *entry)
{
    uint32_t keyhash = hashtable->hash_function(entry->key, entry->keylen, hashtable->hash_seed);
    uint32_t nbin = octo_hash_nbin(hashtable->n_hash_bins, keyhash);
    octo_hash_entry *head = hashtable->hash_bins[nbin];

    if(head == NULL)
    {
        hashtable->size += 1;
        hashtable->hash_bins[nbin] = entry;
        return true;
    }

    octo_hash_entry *existing = octo_hash_bin_get(head, entry->key, entry->keylen);

    if(existing)
    {
        return false;
    }

    hashtable->hash_bins[nbin] = entry;
    entry->next = head;

    hashtable->size += 1;
    return true;
}

inline octo_hash_entry * octo_hash_get(octo_hash *hashtable, void *key, size_t keylen)
{
    uint32_t keyhash = hashtable->hash_function(key, keylen, hashtable->hash_seed);
    uint32_t nbin = octo_hash_nbin(hashtable->n_hash_bins, keyhash);
    octo_hash_entry *entry = hashtable->hash_bins[nbin]; 

    return octo_hash_bin_get(entry, key, keylen);
}

inline octo_hash_entry * octo_hash_pop(octo_hash *hashtable, void *key, size_t keylen)
{
    uint32_t keyhash = hashtable->hash_function(key, keylen, hashtable->hash_seed);
    uint32_t nbin = octo_hash_nbin(hashtable->n_hash_bins, keyhash);
    octo_hash_entry *cur = hashtable->hash_bins[nbin];
    octo_hash_entry *prev = NULL;

    while(cur != NULL)
    {
        if(keylen != cur->keylen)
        {
            continue;
        }
        
        if(memcmp(cur->key, key, keylen) == 0)
        {
            if(prev)
            {
                prev->next = cur->next;
            }
            else
            {
                hashtable->hash_bins[nbin] = cur->next;
            }

            cur->next = NULL;
            hashtable->size -= 1;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }

    return NULL;
}
