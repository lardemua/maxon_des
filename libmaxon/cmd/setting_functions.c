#include "cmd.h"

void cmd_speed(cmd_context *context, int speed)
{
    des_error err = des_set_velocity(context->des, speed);
    if (err)
    {
        printf("error setting velocity: %s\n", des_strerror(err));

        exit(-1);
    }
    printf("setting velocity succeded!!!\n");
}

void cmd_current(cmd_context *context,int current)
{
    des_error err=des_set_current(context->des,current);
    if (err){
        printf("error setting current: %s\n",des_strerror(err));
        exit(-1);
    }
    printf("setting current succeded!!!\n");
}

void cmd_stop(cmd_context *context)
{
    des_error err = des_stop_motion(context->des);
    if (err)
    {
        return;
    }
}

