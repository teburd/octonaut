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
#include <stdbool.h>
#include <stdint.h>
#include <ev.h>

#include "buffer.h"

/**
 * octo_aio 
 *
 * Provides intelligently buffered asynchronous non-blocking IO on a file
 * descriptor. Buffers on write only when needed otherwise simply writes
 * directly to the socket.
 */
typedef struct octo_aio octo_aio;
typedef ssize_t (*octo_aio_write_cb) (void *ctx, void *data, size_t len);
typedef void (*octo_aio_read_cb) (void *ctx, void *data, size_t len);
typedef void (*octo_aio_close_cb) (void *ctx, bool error);

struct octo_aio {
    struct ev_loop *loop;
    int fd;
    ev_io read_watcher;
    ev_io write_watcher;
    octo_buffer write_buffer;
    octo_aio_write_cb write;
    void *write_ctx;
    octo_aio_read_cb read;
    void *read_ctx;
    octo_aio_close_cb close;
    void *close_ctx;
};

void octo_aio_init(octo_aio *s, struct ev_loop *loop, int fd);
void octo_aio_destroy(octo_aio *s);

void octo_aio_start(octo_aio *s);
void octo_aio_stop(octo_aio *s);
ssize_t octo_aio_write(octo_aio *s, void *data, size_t len);
void octo_aio_close(octo_aio *s);


/**
 * buffered and direct write callbacks, the defaults but can be
 * changed as desired!
 */
ssize_t octo_aio_buffered_write(void *ctx, void *data, size_t len);
ssize_t octo_aio_direct_write(void *ctx, void *data, size_t len);
