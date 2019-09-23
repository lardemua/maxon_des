#include <stdlib.h>
#include <stdio.h>

#include "mx_des.h"

int main()
{
    des_init_params params;
    params.port = "/dev/ttyUSB0";

    des_context *context = des_init(&params);
    if (context == NULL)
    {
        printf("error initializing context\n");
        exit(-1);
    }

    // des_sys_errors errors;

    // des_error err = des_read_sys_errors(context, &errors);
    // if (err)
    // {
    //     printf("error reading errors: %d\n", err);
    //     exit(-1);
    // }

    // printf("errors: %x\n", errors.value);
    // printf("  hall_sensor = %d\n", errors.flags.hall_sensor);
    // printf("  index_processing = %d\n", errors.flags.index_processing);
    // printf("  encoder_resolution = %d\n", errors.flags.encoder_resolution);
    // printf("  hall_sensor_not_found = %d\n", errors.flags.hall_sensor_not_found);
    // printf("  over_current = %d\n", errors.flags.over_current);
    // printf("  over_voltage = %d\n", errors.flags.over_voltage);
    // printf("  over_speed = %d\n", errors.flags.over_speed);
    // printf("  supply_voltage_low = %d\n", errors.flags.supply_voltage_low);
    // printf("  angle_detection = %d\n", errors.flags.angle_detection);
    // printf("  parameter_out_of_range = %d\n", errors.flags.parameter_out_of_range);
    // printf("  errors = %d\n", errors.flags.errors);

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

    des_error err;
    err = des_enable(context, true);
    if (err)
    {
        printf("error enabling board: %d\n", err);
        exit(-1);
    }
    printf("enable succeded\n");

    err = des_set_velocity(context, 1000);
    if (err)
    {
        printf("error setting velocity: %d\n", err);

        exit(-1);
    }
    printf("setting velocity succeded!!!\n");

    err = des_stop_motion(context);

    if (err)
    {
        printf("error stopping: %d\n", err);
        exit(-1);
    }

    return 0;
}
