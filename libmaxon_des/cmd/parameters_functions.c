#include "cmd.h"

#include <string.h>
#include <assert.h>

typedef struct
{
    const char *identifier;
    const char *name;
} parameter_detail;

void cmd_version(cmd_context *context)
{
    assert(context != NULL);

    des_version_info version;
    des_error err = des_read_version(context->des, &version);
    if (err != 0)
    {
        log_error("error reading version number: %d", err);

        return;
    }

    printf("version:\n");
    printf("  hardware version: 0x%x\n", version.hardware_version);
    printf("  software version: 0x%x\n", version.software_version);
}

static char *parameter_indexes[48] = {
    "baudrate",
    "sys_config",
    "current_regulation_pgain",
    "current_regulation_igain",
    "max_output_current_re",
    "speed_regulator_pgain3",
    "speed_regulato",
    "intern_param1",
    "intern_param2",
    "intern_param3",
    "max_speed",
    "setting_unit_gain",
    "setting_unit_offset",
    "setting_unit_delay",
    "peak_current",
    "max_const_current",
    "therm_const",
    "max_speed",
    "acceleration",
    "speed_constant",
    "enc_resolution",
    "pole_pair_number",
    "internal_param4",
    "rpm_to_qc_factor",
    "index_offset",
    "pwm_period",
    "max_duty_cycle",
    "current_detection_offset_phase_u",
    "current_detection_offset_phase_v",
    "ad_converter",
    "can_module_id",
    "can_service_id",
    "can_rx_pdo_id",
    "can_tx_pdo_id",
    "can_bcr1",
    "can_bcr2",
    "can_opmode",
    "can_rx_sdo_id",
    "can_tx_sdo_id",
    "can_rtr0_id",
    "can_rtr1_id",
    "can",
    "intern_param5",
    "error_proc",
    "max_speed_curr",
    "hall_angle_offset",
    "max_angle_mpy1",
    "max_angle"};
size_t num_parameters = 47;
uint16_t parameters_mapping(const char *name)
{
    for (int i = 0; i < num_parameters; i++)
    {
        if (strcmp(parameter_indexes[i], name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void cmd_read_temp_param(cmd_context *context, char *param)
{
    assert(context != NULL);
    des_error err;
    des_sys_params parameters;

    uint16_t param_idx = parameters_mapping(param);

    if (param_idx == -1)
    {
        printf("parameter does not exist.\n");

        return;
    }

    uint16_t value;

    err = des_read_temp_param(context->des, param_idx, &value);
    if (err)
    {
        printf("error getting temporary parameter: %s\n", des_strerror(err));
        exit(-1);
    }
    printf("temporary parameter:\n");
    printf("  parameter value: %d\n", value);
}

void cmd_set_temp_param(cmd_context *context, char *param, int value)
{
    assert(context != NULL);
    des_error err;
    des_sys_params parameters;

    uint16_t param_idx = parameters_mapping(param);

    if (param_idx == -1)
    {
        printf("parameter does not exist.\n");

        return;
    }
    err = des_set_temp_param(context->des, param_idx, value);
    if (err)
    {
        printf("error setting temporary parameter: %s\n", des_strerror(err));
        exit(-1);
    }
}

void cmd_reset_temp_param(cmd_context *context)
{
    des_error err = des_reset_temp_param(context->des);
    if (err)
    {
        return;
    }
}

void cmd_save_temp_param(cmd_context *context)
{
    des_error err = des_save_temp_param(context->des);
    if (err)
    {
        return;
    }
}

void cmd_read_all_temp_param(cmd_context *context)
{
    des_sys_params p;
    des_error err;
    err = des_read_all_temp_param(context->des, &p);
    if (err)
    {
        printf("error reading temporary parameters: %d\n", err);
        exit(-1);
    }
    printf("system parameters\n");
    printf("  baudrate = %d\n", p.baudrate);
    printf("  sys_config:\n");
    des_sys_config sc = p.sys_config;
    printf("    speed_current_setting: %s\n", sc.flags.speed_current_setting ? "by analogue input" : "by software");
    printf("    acceleration_config: %d\n", sc.flags.acceleration_config);
    printf("    current_regulator: %d\n", sc.flags.current_regulator);
    printf("    speed_regulator: %d\n", sc.flags.speed_regulator);
    printf("    monitor_input_signal: %d\n", sc.flags.monitor_input_signal);
    printf("    stop_motor_config: %d\n", sc.flags.stop_motor_config);
    printf("    maximum_speed_config: %d\n", sc.flags.maximum_speed_config);
    printf("    maximum_current_config: %d\n", sc.flags.maximum_current_config);
    printf("    regulation_gains_config: %d\n", sc.flags.regulation_gains_config);
    printf("    enable_system_config: %d\n", sc.flags.enable_system_config);
    printf("    monitor_config: %d\n", sc.flags.monitor_config);
    printf("    service_mode_config: %d\n", sc.flags.service_mode_config);
    printf("    regulation_mode_config: %d\n", sc.flags.regulation_mode_config);
    printf("\n");
}

void cmd_set_default_param(cmd_context *context)
{
    des_error err = des_set_default_param(context->des);
    if (err)
    {
        return;
    }
}