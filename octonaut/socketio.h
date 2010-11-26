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

#include <ev.h>

/**
 * SocketIO
 *
 * Provides intelligently buffered non-blocking IO.
 * Buffers on write only when needed otherwise simply writes directly to the
 * socket.
 */
typedef (void*)(socketio_write)(SocketIO* socketio, uint8_t* data, size_t len);
typedef (void*)(socketio_read)(SocketIO* socketio, uint8_t* data, size_t len);
typedef (void*)(socketio_close)(SocketIO* socketio, SocketIOError* error);


typedef struct {
    ev_loop* loop;
    ev_io* read_watcher;
    ev_io* write_watcher;
    size_t buffer_size;
    uint8_t* buffer;
} SocketIO;

void socketio_init(SocketIO* socketio, ev_loop* loop, size_t buffer_size);
void socketio_destroy(SocketIO* socketio);

void socketio_start(SocketIO* socketio);
void socketio_stop(SocketIO* socketio);
void socketio_write(SocketIO* socketio, uint8_t* data, size_t len);
void socketio_close(SocketIO* socketio);
