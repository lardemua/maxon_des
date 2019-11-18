#include "mx_des.h"
#include "maxon_des_functions.h"

des_error des_service(des_context *context, const char password[4], bool *success)
{
    assert(context != NULL);
    assert(success != NULL);

    __SEND_FRAME(0x10, 2,
                  (password[0] << 8) | password[1],
                  (password[0] << 8) | password[1]);

    des_frame frame;

    __RECEIVE_FRAME(frame);

    __VALIDATE_FRAME(frame, 1);

    if (frame.data[0] == 'O')
        *success = true;
    else if (frame.data[0] == 'F')
        *success = false;
    else
    {
        free(frame.data);
        return DES_BAD_RESPONSE;
    }

    free(frame.data);

    return DES_OK;
}

