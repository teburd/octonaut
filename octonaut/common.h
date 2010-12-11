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

#ifndef OCTO_COMMON_H
#define OCTO_COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * macro that obtains a type * from a pointer to a member of type
 */
#define ptr_offset(ptr, type, member) (type *) (((char *)ptr) - offsetof(type, member))

/**
 * min macro
 */
#define min(x, y) ((x)<(y)?(x):(y))

/**
 * max macro
 */
#define max(x, y) ((x)>(y)?(x):(y))

/**
 * returns true if x is a power of 2
 */
static inline bool power_of_two(uint32_t x)
{
    return (x!=0) && ((x&(x-1)) == 0);
}

#endif
