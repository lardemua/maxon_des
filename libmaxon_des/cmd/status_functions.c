#include "cmd.h"

void cmd_reset(cmd_context *context)
{
    des_error err = des_reset(context->des);
    if (err)
    {
        return;
    }
}

void cmd_enable(cmd_context *context)
{
    des_error err = des_enable(context->des, true);
    if (err)
    {
        return;
    }
}