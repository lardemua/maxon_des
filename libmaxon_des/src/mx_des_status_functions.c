#include "mx_des_functions.h"

DES_FN_NO_PARAM_RET_STATUS_STRUCT(des_read_sys_status, 0x01, des_sys_status);

DES_FN_NO_PARAM_RET_STATUS_STRUCT(des_read_sys_errors, 0x02, des_sys_errors);

DES_FN_NO_PARAM_NO_RET(des_clear_errors, 0x03)

DES_FN_NO_PARAM_NO_RET(des_reset, 0x04)

DES_FN_ONE_PARAM_NO_RET(des_enable, 0x05, bool, state, state ? 1 : 0)