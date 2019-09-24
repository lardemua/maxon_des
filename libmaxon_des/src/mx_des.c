#include "mx_des.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mx_des_crc.h"
#include "mx_des_comm.h"

des_context *des_init(des_init_params *params)
{
    des_context *context = malloc(sizeof(des_context));

    des_init_crc();

    des_error err = des_init_comm(context, params->port);
    if (err)
    {
        free(context);
        return NULL;
    }

    return context;
}

void des_quit(des_context *context)
{
    assert(context != NULL);

    des_quit_comm(context);

    free(context);
}

char *des_strerror(des_error error)
{
    assert(error != DES_OK);

    switch (error)
    {
    case DES_COMM_ERROR:
        return "communication error";
    case DES_READ_ERROR:
        return "read error";
    case DES_WRITE_ERROR:
        return "write error";
    case DES_READ_TIMEOUT:
        return "read timeout";
    case DES_COMM_ACK_FAIL:
        return "acknowledge failed";
    case DES_RECEIVE_BAD_OPCODE:
        return "bad opcode received";
    case DES_RECEIVE_BAD_CRC:
        return "bad crc received";
    case DES_BAD_RESPONSE:
        return "bad response";
    default:
        return "unknown error";
    }
}