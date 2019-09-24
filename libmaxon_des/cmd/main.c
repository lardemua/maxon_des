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
    else if (strcmp(argv[1], "param") == 0)
    {
        if (argc != 3)
        {
            printf("missing the speed value.\n");
            return -1;
        }
        char *param;
        strcpy(param, argv[2]);
        cmd_read_tmp_param(&context, param);
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

    // cmd_version(&context);
    // cmd_errors(&context);

    // cmd_speed(&context, 0);

    // des_sys_status status;
    // status.value = 0x01;
    // err = des_read_sys_status(context, &status);
    // if (err)
    // {
    //     printf("error reading errors: %d\n", err);
    //     exit(-1);
    // }

    // printf("status = %x\n", status.value);
    // printf("  encoder_index_status = %d\n", status.flags.encoder_index_status);
    // printf("  hall_sensor_status = %d\n", status.flags.hall_sensor_status);
    // printf("  rotor_position_status = %d\n", status.flags.rotor_position_status);
    // printf("  eeprom_write_enable = %d\n", status.flags.eeprom_write_enable);
    // printf("  max_current_status = %d\n", status.flags.max_current_status);
    // printf("  current_region_status = %d\n", status.flags.current_region_status);
    // printf("  errors_status = %d\n", status.flags.errors_status);
    // printf("  software_enabled = %d\n", status.flags.software_enabled);
    // printf("  enable_input_debuncing_enable = %d\n", status.flags.enable_input_debuncing_enable);
    // printf("  offset_current_detected = %d\n", status.flags.offset_current_detected);
    // printf("  breaking_status = %d\n", status.flags.breaking_status);
    // printf("  power_stage_status = %d\n", status.flags.power_stage_status);

    // printf("\n");

    // des_version_info version;
    // err = des_read_version(context, &version);
    // if (err != 0)
    // {
    //     printf("error reading errors: %d\n", err);
    // }

    // printf("\nhardware version: 0x%x\n", version.hardware_version);
    // printf("software version: 0x%x\n", version.software_version);

    // des_sys_params p;

    // err = des_read_all_temp_param(context, &p);
    // if (err)
    // {
    //     printf("error reading params: %d\n", err);
    //     exit(-1);
    // }

    // printf("system parameters\n");
    // printf("  baudrate = %d\n", p.baudrate);
    // printf("  sys_config:\n");
    // des_sys_config sc = p.sys_config;
    // printf("    speed_current_setting: %s\n", sc.flags.speed_current_setting ? "by analogue input" : "by software");
    // printf("    acceleration_config: %d\n", sc.flags.acceleration_config);
    // printf("    current_regulator: %d\n", sc.flags.current_regulator);
    // printf("    speed_regulator: %d\n", sc.flags.speed_regulator);
    // printf("    monitor_input_signal: %d\n", sc.flags.monitor_input_signal);
    // printf("    stop_motor_config: %d\n", sc.flags.stop_motor_config);
    // printf("    maximum_speed_config: %d\n", sc.flags.maximum_speed_config);
    // printf("    maximum_current_config: %d\n", sc.flags.maximum_current_config);
    // printf("    regulation_gains_config: %d\n", sc.flags.regulation_gains_config);
    // printf("    enable_system_config: %d\n", sc.flags.enable_system_config);
    // printf("    monitor_config: %d\n", sc.flags.monitor_config);
    // printf("    service_mode_config: %d\n", sc.flags.service_mode_config);
    // printf("    regulation_mode_config: %d\n", sc.flags.regulation_mode_config);

    // des_clear_errors(context);

    // err = des_reset(context);
    // if (err)
    // {
    //     printf("error resetting context: %d\n", err);
    // }

    // err = des_enable(context, true);
    // if (err)
    // {
    //     printf("error enabling board: %d\n", err);
    //     exit(-1);
    // }
    // printf("enable succeded\n");

    // err = des_stop_motion(context);

    // if (err)
    // {
    //     printf("error stopping: %d\n", err);
    //     exit(-1);
    // }

    return 0;
}
