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

void cmd_version(cmd_context *context);
void cmd_errors(cmd_context *context);
void cmd_reset(cmd_context *context);
void cmd_enable(cmd_context *context);

void cmd_speed(cmd_context *context, int speed);
void cmd_stop(cmd_context *context);

#endif // MAXON_DES_CMD_H