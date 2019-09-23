#include "cmd.h"

void cmd_errors(cmd_context *context)
{
    des_sys_errors errors;

    des_error err = des_read_sys_errors(context->des, &errors);
    if (err)
    {
        printf("error reading errors: %d\n", err);
        exit(-1);
    }

    printf("errors: %x\n", errors.value);
    printf("  hall_sensor = %d\n", errors.flags.hall_sensor);
    printf("  index_processing = %d\n", errors.flags.index_processing);
    printf("  encoder_resolution = %d\n", errors.flags.encoder_resolution);
    printf("  hall_sensor_not_found = %d\n", errors.flags.hall_sensor_not_found);
    printf("  over_current = %d\n", errors.flags.over_current);
    printf("  over_voltage = %d\n", errors.flags.over_voltage);
    printf("  over_speed = %d\n", errors.flags.over_speed);
    printf("  supply_voltage_low = %d\n", errors.flags.supply_voltage_low);
    printf("  angle_detection = %d\n", errors.flags.angle_detection);
    printf("  parameter_out_of_range = %d\n", errors.flags.parameter_out_of_range);
    printf("  errors = %d\n", errors.flags.errors);
}