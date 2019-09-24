#ifndef MX_DES_FUNCTIONS_H
#define MX_DES_FUNCTIONS_H

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#include "mx_des.h"
#include "mx_des_comm.h"

#define __SEND_FRAME(OPCODE, LEN, ...)                \
    {                                                  \
        des_frame frame;                               \
        frame.opcode = (uint8_t)OPCODE;                \
        frame.len = LEN;                               \
        if (frame.len > 0)                             \
            frame.data = (uint16_t[LEN]){__VA_ARGS__}; \
        else                                           \
            frame.data = NULL;                         \
                                                       \
        des_error err;                                 \
                                                       \
        err = des_send_frame(context, &frame);         \
        if (err)                                       \
        {                                              \
            return err;                                \
        }                                              \
    }

#define __RECEIVE_FRAME(frame)                              \
    {                                                       \
        des_error err = des_receive_frame(context, &frame); \
        if (err)                                            \
        {                                                   \
            return err;                                     \
        }                                                   \
    }

#define __VALIDATE_FRAME(FRAME, LEN) \
    {                                \
        if (FRAME.len != LEN)        \
        {                            \
            return DES_BAD_RESPONSE; \
        }                            \
    }

#define DES_FN_NO_PARAM_NO_RET(NAME, OPCODE) \
    des_error NAME(des_context *context)     \
    {                                        \
        assert(context != NULL);             \
                                             \
        __SEND_FRAME(OPCODE, 0);             \
                                             \
        return DES_OK;                       \
    }

#define DES_FN_ONE_PARAM_NO_RET(NAME, OPCODE, PARAM_TYPE, PARAM_NAME, PARAM_FN) \
    des_error NAME(des_context *context, PARAM_TYPE PARAM_NAME)                 \
    {                                                                           \
        assert(context != NULL);                                                \
                                                                                \
        __SEND_FRAME(OPCODE, 1, PARAM_FN)                                      \
                                                                                \
        return DES_OK;                                                          \
    }

#define DES_FN_NO_PARAM_RET_STATUS_STRUCT(NAME, OPCODE, RET_TYPE) \
    des_error NAME(des_context *context, RET_TYPE *output)        \
    {                                                             \
        assert(context != NULL);                                  \
        assert(output != NULL);                                   \
                                                                  \
        __SEND_FRAME(OPCODE, 0);                                 \
                                                                  \
        des_frame frame;                                          \
                                                                  \
        __RECEIVE_FRAME(frame)                                    \
                                                                  \
        __VALIDATE_FRAME(frame, 1)                                \
                                                                  \
        output->value = frame.data[0];                            \
                                                                  \
        free(frame.data);                                         \
                                                                  \
        return DES_OK;                                            \
    }

#define DES_FN_NO_PARAM_RET_STRUCT(NAME, OPCODE, RET_STRUCT, LEN) \
    des_error NAME(des_context *context, RET_STRUCT *output)      \
    {                                                             \
        assert(context != NULL);                                  \
        assert(output != NULL);                                   \
                                                                  \
        __SEND_FRAME(OPCODE, 0);                                 \
                                                                  \
        des_frame frame;                                          \
                                                                  \
        __RECEIVE_FRAME(frame)                                    \
                                                                  \
        __VALIDATE_FRAME(frame, LEN)                              \
                                                                  \
        memcpy(output, frame.data, LEN * 2);                      \
                                                                  \
        free(frame.data);                                         \
                                                                  \
        return DES_OK;                                            \
    }

#endif // MX_DES_FUNCTIONS_H