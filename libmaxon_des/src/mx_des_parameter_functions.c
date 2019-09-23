#include "mx_des.h"
#include "mx_des_functions.h"

DES_FN_NO_PARAM_RET_STRUCT(des_read_all_temp_param, 0x18, des_sys_params, 47)

DES_FN_NO_PARAM_RET_STRUCT(des_read_version, 0x1A, des_version_info, 4)

DES_FN_NO_PARAM_NO_RET(des_reset_temp_param, 0x16)

DES_FN_NO_PARAM_NO_RET(des_save_temp_param, 0x17)

DES_FN_NO_PARAM_NO_RET(des_set_default_param, 0x1B)

des_error des_read_temp_param(des_context *context, uint16_t paramNb, bool dataFormat)
{
    assert(context != NULL);
    assert(paramNb != -1);
    assert(dataFormat == 0 || dataFormat == 1);

    des_frame frame;
    frame.opcode = 0x14;
    frame.data[0] = 0x01;
    frame.data[1] = (paramNb & 0xff);
    frame.data[2] = ((paramNb >> 8) & 0xff);
    frame.data[3] = (dataFormat & 0xff);
    frame.data[4] = ((dataFormat >> 8) & 0xff);
    des_error err;
    err = des_send_frame(context, &frame);
    __RECEIVE_FRAME(frame);
    __VALIDATE_FRAME(frame, 1);
    if ((frame.data[0] != 'O') && (frame.data[0] != 'F'))
    {
        free(frame.data);
        return DES_BAD_RESPONSE;
    }

    free(frame.data);

    return DES_OK;
}

des_error des_set_temp_param(des_context *context, uint16_t paramNb, bool dataFormat, uint32_t value)
{

    assert(context != NULL);
    assert(paramNb != -1);
    assert(dataFormat == 0 || dataFormat == 1);

    des_frame frame;
    frame.opcode = 0x15;
    frame.data[1] = (paramNb & 0xff);
    frame.data[2] = ((paramNb >> 8) & 0xff);
    frame.data[3] = (dataFormat & 0xff);
    frame.data[4] = ((dataFormat >> 8) & 0xff);

    if (dataFormat == 0) //send 16 bits
    {
        frame.data[0] = 0x02;
        frame.data[5] = (value & 0xff);
        frame.data[6] = ((value >> 8) & 0xff);
    }
    else if (dataFormat == 1) // send 32 bits
    {
        frame.data[0] = 0x03;
        int low_word = (value & 0xff);
        int higher_word = (value >> 16) & 0xffff;
        frame.data[5] = (higher_word & 0xff);
        frame.data[6] = ((higher_word >> 8) & 0xff);
        frame.data[7] = (low_word & 0xff);
        frame.data[8] = ((low_word >> 8) & 0xff);
    }
    des_error err;
    err = des_send_frame(context, &frame);
    free(frame.data);
    if (err)
    {
        return err;
    }
    else
    {
        return DES_OK;
    }
}
