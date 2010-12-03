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

#include "hash_function.h"

#define rotl64(num,amount) (((num) << (amount)) | ((num) >> (64 - (amount))))

#define murmur3_bmix64(h1, h2, k1, k2, c1, c2) \
    do {\
        k1 *= c1;\
        k1  = rotl64(k1,23);\
        k1 *= c2;\
        h1 ^= k1;\
        h1 += h2;\
\
        h2 = rotl64(h2,41);\
\
        k2 *= c2;\
        k2  = rotl64(k2,23);\
        k2 *= c1;\
        h2 ^= k2;\
        h2 += h1;\
\
        h1 = h1*3+0x52dce729;\
        h2 = h2*3+0x38495ab5;\
\
        c1 = c1*5+0x7b7d159c;\
        c2 = c2*5+0x6bce6396;\
    } while(0)

#define murmur3_fmix64(k) \
    do {\
        k ^= k >> 33;\
        k *= 0xff51afd7ed558ccd;\
        k ^= k >> 33;\
        k *= 0xc4ceb9fe1a85ec53;\
        k ^= k >> 33;\
    } while(0)\


/**
 * murmurhash3 method of hashing for 64 bit machines
 *
 * taken straight from murmurhash3
 * probably fewer collisions but slower and more complex than x31
 */
uint32_t octo_hash_murmur3(const uint8_t *key, const size_t keylen, const uint32_t seed)
{
    const int nblocks = keylen/16;

    uint64_t h1 = 0x9368e53c2f6af274^seed;
    uint64_t h2 = 0x586dcd208f7cd3fd^seed;

    uint64_t c1 = 0x87c37b91114253d5;
    uint64_t c2 = 0x4cf5ad432745937f;

    const uint64_t *blocks = (const uint64_t *)(key);

    for(int i = 0; i < nblocks; i++)
    {
        uint64_t k1 = blocks[i*2+0];
        uint64_t k2 = blocks[i*2+1];

        murmur3_bmix64(h1,h2,k1,k2,c1,c2);
    }

    //----------
    // tail

    const uint8_t *tail = (const uint8_t*)(key + nblocks*16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch(keylen & 15)
    {
        case 15: k2 ^= ((uint64_t)tail[14]) << 48;
        case 14: k2 ^= ((uint64_t)tail[13]) << 40;
        case 13: k2 ^= ((uint64_t)tail[12]) << 32;
        case 12: k2 ^= ((uint64_t)tail[11]) << 24;
        case 11: k2 ^= ((uint64_t)tail[10]) << 16;
        case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
        case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;

        case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
        case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
        case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
        case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
        case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
        case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
        case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
        case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
                 murmur3_bmix64(h1,h2,k1,k2,c1,c2);
    };

    //----------
    // finalization

    h2 ^= keylen;

    h1 += h2;
    h2 += h1;

    murmur3_fmix64(h1);
    murmur3_fmix64(h2);

    h1 += h2;
    h2 += h1;

    uint32_t* hash = (uint32_t*)&h1;
    return hash[0];
}

/**
 * x31 hash, very simple and fast, probably lots of collisions
 */
uint32_t octo_hash_x31(const uint8_t *key, const size_t keylen, const uint32_t seed)
{
    uint32_t mkeylen = keylen;
    uint32_t hash = seed + keylen;
    for ( ; mkeylen & ~1; mkeylen -= 2, key += 2 )
        hash = ( ( ( hash * 31 ) + key[0] ) * 31 ) + key[1];
    if ( mkeylen & 1 )
        hash = ( hash * 31 ) + key[0];
    return hash;
}
