/**
 * @file mx_des.h
 * @version 0.1
 * @date 2019-09-23
 * 
 */

#ifndef MAXON_DES_H
#define MAXON_DES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int sleep;
    int retries;
} des_init_params;

typedef struct
{
    int port;
    int sleep;
    int retries;
} des_context;

typedef struct
{
    // Software version of firmware
    uint16_t software_version;
    // Hardware version of firmware
    uint16_t hardware_version;
    // Application number of firmware
    uint16_t application_number;
    // Application version of firmware
    uint16_t application_version;
} des_version_info;

typedef union {
    uint16_t value;
    struct
    {
        bool encoder_index_status : 1;
        bool hall_sensor_status : 1;
        bool rotor_position_status : 1;
        bool eeprom_write_enable : 1;
        uint8_t __reserved : 3;
        bool max_current_status : 1;
        bool current_region_status : 1;
        bool errors_status : 1;
        bool software_enabled : 1;
        bool enable_input_debuncing_enable : 1;
        bool offset_current_detected : 1;
        bool breaking_status : 1;
        uint8_t power_stage_status : 2;
    } flags;
} des_sys_status;

typedef union {
    uint16_t value;
    struct
    {
        bool hall_sensor : 1;
        bool index_processing : 1;
        bool encoder_resolution : 1;
        bool hall_sensor_not_found : 1;
        bool over_current : 1;
        bool over_voltage : 1;
        bool over_speed : 1;
        bool supply_voltage_low : 1;
        bool angle_detection : 1;
        bool parameter_out_of_range : 1;
        bool errors : 1;
    } flags;
} des_sys_errors;

typedef union {
    uint16_t value;
    struct
    {
        bool speed_current_setting : 1;
        bool acceleration_config : 1;
        bool current_regulator : 1;
        bool speed_regulator : 1;
        bool monitor_input_signal : 1;
        uint8_t __reserved : 2;
        bool stop_motor_config : 1;
        bool maximum_speed_config : 1;
        bool maximum_current_config : 1;
        bool regulation_gains_config : 1;
        bool enable_system_config : 1;
        bool monitor_config : 1;
        bool service_mode_config : 1;
        bool regulation_mode_config : 1;
    } flags;
} des_sys_config;

typedef struct
{
    uint16_t baudrate;
    des_sys_config sys_config;

    uint16_t current_regulation_pgain;
    uint16_t current_regulation_igain;
    uint16_t max_output_current_regulator;

    uint16_t speed_regulator_pgain;
    uint16_t speed_regulator_igain;

    uint16_t intern_param1;
    uint16_t intern_param2;
    uint16_t intern_param3;
    uint16_t max_speed_error;

    uint16_t setting_unit_gain;
    uint16_t setting_unit_offset;
    uint16_t setting_unit_delay;
    uint16_t peak_current;
    uint16_t max_const_current;
    uint16_t therm_const;
    uint16_t max_speed;
    uint16_t acceleration;
    uint16_t speed_constant;
    uint16_t enc_resolution;
    uint16_t pole_pair_number;
    uint16_t internal_param4;
    uint16_t rpm_to_qc_factor;
    uint16_t index_offset;
    uint16_t pwm_period;
    uint16_t max_duty_cycle;
    uint16_t current_detection_offset_phase_u;
    uint16_t current_detection_offset_phase_v;
    uint16_t ad_converter_offset;

    uint16_t can_module_id;
    uint16_t can_service_id;
    uint16_t can_rx_pdo_id;
    uint16_t can_tx_pdo_id;
    uint16_t can_bcr1;
    uint16_t can_bcr2;
    uint16_t can_opmode;
    uint16_t can_rx_sdo_id;
    uint16_t can_tx_sdo_id;
    uint16_t can_rtr0_id;
    uint16_t can_rtr1_id;
    uint16_t can_config;

    uint16_t intern_param5;
    uint16_t error_proc;
    uint16_t max_speed_curr;
    uint16_t hall_angle_offset;
    uint16_t max_angle_mpy1;
    uint16_t max_angle_mpyn;
} des_sys_params;

typedef enum
{
    DES_OK = 0,
    DES_COMM_ERROR,
    DES_READ_ERROR,
    DES_WRITE_ERROR,
    DES_READ_TIMEOUT,
    DES_COMM_ACK_FAIL,
    DES_RECEIVE_BAD_OPCODE,
    DES_RECEIVE_BAD_CRC,
    DES_BAD_RESPONSE,
} des_error;

des_context *des_init(char *port, des_init_params *params);

void des_quit(des_context *context);

char *des_strerror(des_error error);

///
/// Status functions
///

des_error des_read_sys_status(des_context *context, des_sys_status *status);

des_error des_read_sys_errors(des_context *context, des_sys_errors *errors);

des_error des_clear_errors(des_context *context);

des_error des_reset(des_context *context);

/**
 * @brief Sets the system into enable and disabled state.
 * 
 * Set the system into the enabled or disabled state. The DES has to be configured
 * for a software setting of Enable. If the hardware Enable is activated this
 * command has no effect.
 * 
 * @param context the DES context
 * @param state the enable (true) or disable (false) to set.
 * @return the result error, if occurred
 */
des_error des_enable(des_context *context, bool state);

/// Service functions

des_error des_service(des_context *context, const char password[4], bool *success);

des_error des_set_addr_var(des_context *context, uint16_t address, bool lword, uint32_t value);

des_error des_read_addr_var(des_context *context, uint16_t address, bool lword, uint32_t *value);

/// System parameter functions

des_error des_set_temp_param(des_context *context, uint16_t param, uint16_t value);

des_error des_read_temp_param(des_context *context, uint16_t param, uint16_t * value);

des_error des_reset_temp_param(des_context *context);

des_error des_save_temp_param(des_context *context);

des_error des_read_all_temp_param(des_context *context, des_sys_params *params);

des_error des_set_all_temp_param(des_context *context, des_sys_params *params);

des_error des_read_version(des_context *context, des_version_info *version_info);

des_error des_set_default_param(des_context *context);

/// Setting Functions

des_error des_set_velocity(des_context *context, int16_t velocity);

des_error des_set_current(des_context *context, int16_t current);

des_error des_stop_motion(des_context *context);

/// Monitor Functions

typedef struct
{
    short real;
    short requested;
} des_reading;

des_error des_read_velocity(des_context *context, bool type, des_reading *reading);

des_error des_read_current(des_context *context, bool type, des_reading *reading);

///
/// Data Recording Functions
///

/**
 * @brief Start recording data.
 * 
 * Start recording data. The sampling starts immediately after a jump of the setting
 * value. The recording is stopped after 256 samples. The current jump is only
 * executed if the DES configured for a digital setting value.
 * 
 * @param context
 * @param sampling Sampling period as a factor of 0.1ms.
 * @param variable Number of a system parameter or a numbered status
 *                 variable. If the number is greater than 0x0300 it represents
 *                 a memory address.
 * @return des_error 
 */
des_error des_setup_recorder(des_context *context, uint16_t sampling, uint16_t variable);

des_error des_record_data(des_context *context, uint16_t jump);

des_error des_read_variables(des_context *context, uint16_t *variables, uint16_t *data, int len);

#endif // MAXON_DES_H
