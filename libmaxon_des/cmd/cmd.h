#ifndef MAXON_DES_CMD_H
#define MAXON_DES_CMD_H

#define COMMAND "maxon_des"
#define MAINTAINER "demua <demua@ua.pt>"

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

#include "mx_des.h"

typedef struct
{
    des_context *des;
} cmd_context;

cmd_context *cmd_init();
void cmd_quit();


//Status commands
void cmd_status(cmd_context *context);
void cmd_errors(cmd_context *context);
void cmd_reset(cmd_context *context);
void cmd_enable(cmd_context *context);
void cmd_clear_errors(cmd_context *context);

//Parameters commands
void cmd_version(cmd_context *context);
void cmd_read_temp_param(cmd_context *context, char * param);
void cmd_set_temp_param (cmd_context *context, char *param, int value);
void cmd_reset_temp_param(cmd_context *context);
void cmd_save_temp_param(cmd_context *context);
void cmd_read_all_temp_param(cmd_context *context);
void cmd_set_default_param(cmd_context *context);

//Setting commands
void cmd_speed(cmd_context *context, int speed);
void cmd_current(cmd_context *context,int current);
void cmd_stop(cmd_context *context);

#endif // MAXON_DES_CMD_H