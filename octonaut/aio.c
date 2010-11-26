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
#include <stdio.h>

#include "aio.h"

/**
 * callback given to ev_io to be called when the
 * file descriptor is readable.
 */
static void octo_aio_readable(EV_P_ ev_io *watcher, int revents)
{
    octo_aio *aio = (octo_aio*)watcher->data;
}

void octo_aio_init(octo_aio *aio, struct ev_loop *loop, int fd, size_t buffer_size)
{
    aio->loop = loop;
    aio->buffer_size = buffer_size;
    aio->buffer = malloc(buffer_size);
    aio->fd = fd;
    
    aio->read_watcher.data = aio;
    aio->write_watcher.data = aio;
    ev_io_init( &aio->read_watcher, octo_aio_readable, aio->fd, EV_READ);
    ev_io_init( &aio->write_watcher, octo_aio_readable, aio->fd, EV_WRITE);
}

void octo_aio_destroy(octo_aio *aio)
{
    ev_io_stop(aio->loop, &aio->read_watcher);
    ev_io_stop(aio->loop, &aio->write_watcher);

    aio->loop = NULL;
    aio->buffer_size = 0;
    free(aio->buffer);
    aio->fd = -1;
}

void octo_aio_start(octo_aio *aio)
{
    ev_io_start(aio->loop, &aio->read_watcher);
}

void octo_aio_stop(octo_aio *aio)
{
    ev_io_stop(aio->loop, &aio->read_watcher);
}
