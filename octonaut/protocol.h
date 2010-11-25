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

typedef struct {

} Protocol;

typedef (void*)(protocol_connected)(Protocol* protocol, SocketAddress* address);
typedef (void*)(protocol_data)(Protocol* protocol, uint8_t* data, size_t len);
typedef (void*)(protocol_disconnected)(Protocol* protocol, SocketError* error);


typedef struct {

} ProtocolFactory;

typedef (Protocol*)(protocol_factory_buid)(Loop* loop);

void protocol_init(Protocol* protocol, protocol_connected connected,
    protocol_disconnected disconnected, protocol_data data);

void protocol_destroy(Protocol* protocol);

void protocol_connected(Protocol* protocol, SocketAddress* address);
void protocol_data(Protocol* protocol, uint8_t* data, size_t len);
void protocol_disconnected(Protocol* protocol, SocketError* error);
