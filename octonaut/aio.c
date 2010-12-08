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
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "aio.h"

/**
 * callback given to ev_io to be called when the
 * file descriptor is readable.
 */
static void octo_aio_readable(EV_P_ ev_io *watcher, int revents)
{
    octo_aio *aio = (octo_aio*)watcher->data;
    size_t len = 0;
    size_t maxlen = 4096;
    uint8_t buffer[maxlen];
    
    len = read(aio->fd, buffer, maxlen);
    aio->read(aio->read_ctx, buffer, len);
}

/**
 * callback given to ev_io to be called when the file descriptor is
 * writtable.
 */
static void octo_aio_writtable(EV_P_ ev_io *watcher, int revents)
{
    /* if the buffer is not empty, copy data from the buffer
     * to the fd
     */
    uint8_t buffer[4096];
    octo_aio *aio = (octo_aio*)watcher->data;
    size_t len = octo_buffer_peek(&aio->write_buffer, buffer, 4096);
    ssize_t result = write(aio->fd, buffer, len);

    if(result == -1 && errno != EAGAIN)
    {
        perror("write");
    }
    else if(result != -1)
    {
        octo_buffer_drain(&aio->write_buffer, len);
    }

    if(octo_buffer_size(&aio->write_buffer) == 0)
    {
        aio->write = octo_aio_direct_write;
        ev_io_stop(loop, watcher);
    }
}

void octo_aio_init(octo_aio *aio, struct ev_loop *loop, int fd)
{
    aio->loop = loop;
    aio->fd = fd;
    octo_buffer_init(&aio->write_buffer, 0);
    fcntl(aio->fd, F_SETFL, O_NONBLOCK);
  
    aio->write_ctx = aio;
    aio->write = octo_aio_direct_write;

    aio->read_watcher.data = aio;
    aio->write_watcher.data = aio;
    ev_io_init( &aio->read_watcher, octo_aio_readable, aio->fd, EV_READ);
    ev_io_init( &aio->write_watcher, octo_aio_writtable, aio->fd, EV_WRITE);
}

void octo_aio_destroy(octo_aio *aio)
{
    ev_io_stop(aio->loop, &aio->read_watcher);
    ev_io_stop(aio->loop, &aio->write_watcher);

    octo_buffer_destroy(&aio->write_buffer);
    aio->fd = -1;
    aio->loop = NULL;
}

void octo_aio_start(octo_aio *aio)
{
    ev_io_start(aio->loop, &aio->read_watcher);
}

void octo_aio_stop(octo_aio *aio)
{
    ev_io_stop(aio->loop, &aio->read_watcher);
}

ssize_t octo_aio_write(octo_aio *aio, void *data, size_t len)
{
    return aio->write(aio->write_ctx, data, len);
}

ssize_t octo_aio_buffered_write(void *ctx, void *data, size_t len)
{
    /*
     * write data to a buffer that feeds to a file descriptor
     */
    assert((ssize_t)len != -1);


    octo_aio *aio = (octo_aio*)ctx;

    ssize_t result = octo_buffer_write(&aio->write_buffer, data, len);
    return result;
}

ssize_t octo_aio_direct_write(void *ctx, void *rawdata, size_t len)
{
    /*
     * write to the file descriptor the data, if
     * the write doesn't fully write all the data then add it to 
     * a buffer and enable buffered writting until the buffer is
     * cleared
     */

    assert((ssize_t)len != -1);
    
    octo_aio *aio = (octo_aio*)ctx;
    uint8_t *data = (uint8_t*)rawdata;

    ssize_t result = write(aio->fd, data, len);

    if(result == -1)
    {
        if(errno == EAGAIN)
        {
            ev_io_start(aio->loop, &aio->write_watcher);
            aio->write = octo_aio_buffered_write;
            result = aio->write(aio, data, len);
            assert(result == len);
            return result;
        }
        else
        {
            perror("write");
            return result;
        }
    }
    
    if(result < len)
    {
        ev_io_start(aio->loop, &aio->write_watcher);
        aio->write = octo_aio_buffered_write;
        size_t bresult = aio->write(aio, &data[result], len - result);
        assert(bresult == (len - result));
        return len;
    }

    return result;
}
