#include <mx_des_comm.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <memory.h>
#include <cmocka.h>

des_error __wrap_des_send_frame(des_context *context, des_frame *frame)
{

    uint16_t opcode = frame->opcode;
    uint16_t len = frame->len;
    uint16_t *data = frame->data;

    check_expected(opcode);
    check_expected(len);
    check_expected(data);

    return mock();
}

des_error __wrap_des_receive_frame(des_context *context, des_frame *frame)
{
    des_frame *frame_send = (des_frame *)mock();
    frame->opcode = frame_send->opcode;
    frame->len = frame_send->len;
    if (frame->len > 0)
    {
        frame->data = malloc(sizeof(uint16_t) * frame->len);
        memcpy(frame->data, frame_send->data, sizeof(uint16_t) * frame->len);
    }
    else
    {
        frame->data = NULL;
    }

    return mock();
}