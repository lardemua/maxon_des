#include "mx_des.h"
#include "mx_des_functions.h"

DES_FN_NO_PARAM_RET_STRUCT(des_read_all_temp_param, 0x18, des_sys_params, 47)

DES_FN_NO_PARAM_RET_STRUCT(des_read_version, 0x1A, des_version_info, 4)

DES_FN_NO_PARAM_NO_RET(des_reset_temp_param, 0x16)

DES_FN_NO_PARAM_NO_RET(des_save_temp_param, 0x17)

DES_FN_NO_PARAM_NO_RET(des_set_default_param, 0x1B)

des_error des_read_temp_param(des_context *context, uint16_t param, uint16_t *value)
{
    assert(context != NULL);
    assert(param >= 0);
    assert(value !=NULL);

    __SEND_FRAME(0x14, 2, param, 0)

    des_frame frame;

    __RECEIVE_FRAME(frame);
    __VALIDATE_FRAME(frame, 1);

    *value = frame.data[0];

    free(frame.data);

    return DES_OK;
}

des_error des_set_temp_param(des_context *context, uint16_t param, uint16_t value)
{

    assert(context != NULL);
    assert(param != -1);
    __SEND_FRAME(0x15, 3, param, 0, value);
    return DES_OK;
}
