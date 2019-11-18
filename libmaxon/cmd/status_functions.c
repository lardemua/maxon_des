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

void cmd_clear_errors(cmd_context *context)
{
    des_error err = des_reset(context->des);
    if (err)
    {
        return;
    }
}

void cmd_status(cmd_context *context){

    des_error err;
    des_sys_status status;

    err = des_read_sys_status(context->des, &status);
    if (err)
    {
        printf("error reading errors: %d\n", err);
        exit(-1);
    }

    printf("status = %x\n", status.value);
    printf("  encoder_index_status = %d\n", status.flags.encoder_index_status);
    printf("  hall_sensor_status = %d\n", status.flags.hall_sensor_status);
    printf("  rotor_position_status = %d\n", status.flags.rotor_position_status);
    printf("  eeprom_write_enable = %d\n", status.flags.eeprom_write_enable);
    printf("  max_current_status = %d\n", status.flags.max_current_status);
    printf("  current_region_status = %d\n", status.flags.current_region_status);
    printf("  errors_status = %d\n", status.flags.errors_status);
    printf("  software_enabled = %d\n", status.flags.software_enabled);
    printf("  enable_input_debuncing_enable = %d\n", status.flags.enable_input_debuncing_enable);
    printf("  offset_current_detected = %d\n", status.flags.offset_current_detected);
    printf("  breaking_status = %d\n", status.flags.breaking_status);
    printf("  power_stage_status = %d\n", status.flags.power_stage_status);

    printf("\n");
}
