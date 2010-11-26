#include "protocol.h"

/**
 * initialize a Protocol struct
 */
void octo_protocol_init(octo_protocol *p)
{
}

/**
 * destroy a Protocol struct
 *
 * NOTE: Does not free the struct
 */
void octo_protocol_destroy(octo_protocol *p)
{
}

/**
 * callback to a Protocol that it has been connected
 */
void octo_protocol_connected(octo_protocol *p, octo_aio_address *a)
{
    p->connected(p, a);
}

/**
 * callback to a Protocol that data has been received
 */
void octo_protocol_data(octo_protocol *p, uint8_t *data, size_t len)
{
    p->data(p, data, len);
}

/**
 * callback to a Protocol that it has been disconnected
 */
void octo_protocol_disconnected(octo_protocol *p, octo_aio_error *e)
{
    p->disconnected(p, e);
}
