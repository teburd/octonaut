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

#include "socketio.h"


/**
 * Protocol handler
 *
 * Takes a fixed set of possible callbacks to handle events from a socketio.
 * May send things back on the socketio.
 */
typedef (void*)(protocol_connected)(Protocol* protocol, SocketIOAddress* address);
typedef (void*)(protocol_data)(Protocol* protocol, uint8_t* data, size_t len);
typedef (void*)(protocol_disconnected)(Protocol* protocol, SocketIOError* error);

typedef struct {
    protocol_connected connected;
    protocol_data data;
    protocol_disconnected disconnected;
    SocketIO socketio;
} Protocol;

/**
 * Protocol Factory
 *
 * Call to build a protocol object.
 */
typedef (Protocol*)(protocol_factory_buid)(Loop* loop);

typedef struct {
    protocol_factory_build build;
} ProtocolFactory;


void protocol_init(Protocol* protocol);
void protocol_destroy(Protocol* protocol);

void protocol_connected(Protocol* protocol, SocketIOAddress* address);
void protocol_data(Protocol* protocol, uint8_t* data, size_t len);
void protocol_disconnected(Protocol* protocol, SocketIOError* error);
