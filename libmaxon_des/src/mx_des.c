#include "mx_des.h"

#include <stdlib.h>
#include <stdio.h>

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
        printf("error init comm\n");
        return NULL;
    }

    return context;
}
