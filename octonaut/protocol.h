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

#ifndef OCTO_PROTOCOL_H
#define OCTO_PROTOCOL_H

#include "aio.h"


/**
 * Protocol handler
 *
 * Takes a fixed set of possible callbacks to handle events from a octo_aio.
 * May send things back on the octo_aio.
 */
typedef struct octo_protocol octo_protocol;
typedef void (*octo_protocol_connected_cb) (octo_protocol *p, octo_aio_address *a);
typedef void (*octo_protocol_data_cb) (octo_protocol *p, uint8_t *data, size_t len);
typedef void (*octo_protocol_disconnected_cb)(octo_protocol* p, octo_aio_error *e);

struct octo_protocol {
    octo_protocol_connected_cb connected;
    octo_protocol_data_cb data;
    octo_protocol_disconnected_cb disconnected;
    octo_aio io;
};

/**
 * Protocol Factory
 *
 * Call to build a octo_protocol object.
 */
typedef octo_protocol * (*octo_protocol_factory_build) ();

typedef struct octo_protocol_factory {
    octo_protocol_factory_build build;
} octo_protocol_factory; 


void octo_protocol_init(octo_protocol *p);
void octo_protocol_destroy(octo_protocol *p);

void octo_protocol_connected(octo_protocol *p, octo_aio_address *a);
void octo_protocol_data(octo_protocol *p, uint8_t *data, size_t len);
void octo_protocol_disconnected(octo_protocol *p, octo_aio_error *e);

#endif
