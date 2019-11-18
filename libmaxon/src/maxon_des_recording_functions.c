#include "maxon_des.h"
#include "maxon_des_functions.h"

des_error des_setup_recorder(des_context *context, uint16_t sampling, uint16_t variable)
{
    assert(context != NULL);

    __SEND_FRAME(0x30, 2, sampling, variable)

    return DES_OK;
}

des_error des_record_data(des_context *context, uint16_t jump)
{
    assert(context != NULL);

    __SEND_FRAME(0x31, 1, jump)

    return DES_OK;
}

des_error des_read_variables(des_context *context, uint16_t *variables, uint16_t *data, int len)
{
    assert(context != NULL);

    uint16_t fdata[len + 1];
    data[0] = len;
    memcpy(fdata + 1, variables, len * 2);

    des_frame frame;
    frame.opcode = 0x32;
    frame.len = len + 1;
    frame.data = fdata;

    des_error error = des_send_frame(context, &frame);
    if (error)
    {
        return error;
    }

    __RECEIVE_FRAME(frame)
    __VALIDATE_FRAME(frame, len)

    memcpy(data, frame.data, len);

    return DES_OK;
}