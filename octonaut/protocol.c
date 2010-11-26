#include "protocol.h"

/**
 * initialize a Protocol struct
 */
void protocol_init(Protocol* protocol)
{
}

/**
 * destroy a Protocol struct
 *
 * NOTE: Does not free the struct
 */
void protocol_destroy(Protocol* protocol)
{
}

/**
 * callback to a Protocol that it has been connected
 */
void protocol_connected(Protocol* protocol, SocketIOAddress* address)
{
    protocol->connected(protocol, address);
}

/**
 * callback to a Protocol that data has been received
 */
void protocol_recv(Protocol* protocol, uint8_t* data, size_t len)
{
    protocol->recv(protocol, data, len);
}

/**
 * callback to a Protocol that it has been disconnected
 */
void protocol_disconnected(Protocol* protocol, SocketIOError* error)
{
    protocol->disconnected(protocol, error);
}
