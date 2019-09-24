#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mx_des.h"

#include "cmd.h"

int main(int argc, char **argv)
{
    des_init_params params;
    char *port = "/dev/ttyUSB1";

    des_context *des_context = des_init(port, NULL);
    if (des_context == NULL)
    {
        printf("error: context failed to initialize\n");
        exit(-1);
    }

    cmd_context context;
    context.des = des_context;

    if (argc < 2)
    {
        return 0;
    }

    //Status commands
    if (strcmp(argv[1], "status") == 0)
    {
        cmd_status(&context);
    }
    else if (strcmp(argv[1], "errors") == 0)
    {
        cmd_errors(&context);
    }
    else if (strcmp(argv[1], "clear") == 0)
    {
        cmd_clear_errors(&context);
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        cmd_reset(&context);
    }
    else if (strcmp(argv[1], "enable") == 0)
    {
        cmd_enable(&context);
    }
    //Parameters commands:
    else if (strcmp(argv[1], "version") == 0)
    {
        cmd_version(&context);
    }
    else if (strcmp(argv[1], "get_param") == 0)
    {
        if (argc != 3)
        {
            printf("missing the param name.\n");
            return -1;
        }
        char *param;
        param = argv[2];
        cmd_read_temp_param(&context, param);
    }
    else if (strcmp(argv[1], "set_param") == 0)
    {
        if (argc != 4)
        {
            printf("missing the param value or the param name.\n");
            return -1;
        }
        char *param;
        int value;
        param = argv[2];
        value = atoi(argv[3]);
        cmd_set_temp_param(&context, param, value);
    }
    else if (strcmp(argv[1], "reset_temp_params") == 0)
    {
        cmd_reset_temp_param(&context);
    }
    else if (strcmp(argv[1], "save_temp_params") == 0)
    {
        cmd_save_temp_param(&context);
    }
    else if (strcmp(argv[1], "read_temp_param") == 0)
    {
        cmd_read_all_temp_param(&context);
    }
    else if(strcmp(argv[1], "set_param_default") == 0)
    {
        cmd_set_default_param(&context);
    }
    //Setting commands:
    else if (strcmp(argv[1], "speed") == 0)
    {
        if (argc != 3)
        {
            printf("missing the speed value.\n");
            return -1;
        }

        int speed = atoi(argv[2]);

        cmd_speed(&context, speed);
    }
    else if (strcmp(argv[1], "current") == 0)
    {
        if (argc != 3)
        {
            printf("missing the current value.\n");
            return -1;
        }

        int current = atoi(argv[2]);

        cmd_current(&context, current);
    }

    else if (strcmp(argv[1], "stop") == 0)
    {
        cmd_stop(&context);
    }
    ///////////////////////////////////////////////////////////////////////////////////
    else
    {
        printf("This command does not exist in the command list. Check the help.\n");
        return 0;
    }

    return 0;
}
