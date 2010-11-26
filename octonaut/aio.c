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

#include "aio.h"

/**
 * callback given to ev_io to be called when the
 * file descriptor is readable.
 */
static void octo_aio_readable(EV_P_ ev_io *watcher, int revents)
{
    octo_aio *a = (octo_aio*)watcher->data;
}

void octo_aio_init(octo_aio *s, struct ev_loop *loop, int fd, size_t buffer_size)
{
    s->loop = loop;
    s->buffer_size = buffer_size;
    s->buffer = malloc(buffer_size);
    s->fd = fd;
    
    ev_io_init( &s->read_watcher, octo_aio_readable, s->fd, EV_READ);
    ev_io_init( &s->write_watcher, octo_aio_readable, s->fd, EV_WRITE);
}
