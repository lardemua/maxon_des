#include "mx_des.h"
#include "maxon_des_functions.h"

des_error des_read_velocity(des_context *context, bool type, des_reading *reading)
{
    assert(context != NULL);
    assert(reading != NULL);

    __SEND_FRAME(0x28, 1, type & 0x1)

    des_frame frame;

    __RECEIVE_FRAME(frame)
    __VALIDATE_FRAME(frame, 2)

    memcpy(reading, frame.data, 4);

    return DES_OK;
}

des_error des_read_current(des_context *context, bool type, des_reading *reading)
{
    assert(context != NULL);
    assert(reading != NULL);

    __SEND_FRAME(0x28, 1, type & 0x1)

    des_frame frame;

    __RECEIVE_FRAME(frame)
    __VALIDATE_FRAME(frame, 2)

    memcpy(reading, frame.data, 4);

    return DES_OK;
}
