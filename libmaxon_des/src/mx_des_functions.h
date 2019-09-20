#ifndef MX_DES_FUNCTIONS_H
#define MX_DES_FUNCTIONS_H

#define DES_FN_NO_PARAM_NO_RET(NAME, OPCODE)   \
    des_error NAME(des_context *context)       \
    {                                          \
        assert(context != NULL);               \
                                               \
        des_frame frame = {                    \
            .opcode = OPCODE,                  \
            .len = 0,                          \
            .data = NULL,                      \
        };                                     \
                                               \
        des_error err;                         \
                                               \
        err = des_send_frame(context, &frame); \
        if (err)                               \
        {                                      \
            return err;                        \
        }                                      \
                                               \
        return DES_OK;                         \
    }

#define DES_FN_ONE_PARAM_NO_RET(NAME, OPCODE, PARAM_TYPE, PARAM_NAME, PARAM_FN) \
    des_error NAME(des_context *context, PARAM_TYPE PARAM_NAME)                 \
    {                                                                           \
        assert(context != NULL);                                                \
                                                                                \
        des_frame frame = {                                                     \
            .opcode = OPCODE,                                                   \
            .len = 1,                                                           \
            .data = (uint16_t[1]){PARAM_FN},                                    \
        };                                                                      \
                                                                                \
        des_error err;                                                          \
                                                                                \
        err = des_send_frame(context, &frame);                                  \
        if (err)                                                                \
        {                                                                       \
            return err;                                                         \
        }                                                                       \
                                                                                \
        return DES_OK;                                                          \
    }

#define DES_FN_NO_PARAM_RET_STATUS_STRUCT(NAME, OPCODE, RET_TYPE) \
    des_error NAME(des_context *context, RET_TYPE *output)        \
    {                                                             \
        assert(context != NULL);                                  \
        assert(output != NULL);                                   \
                                                                  \
        des_frame frame = {                                       \
            .opcode = OPCODE,                                     \
            .len = 0,                                             \
            .data = NULL,                                         \
        };                                                        \
                                                                  \
        des_error err;                                            \
                                                                  \
        err = des_send_frame(context, &frame);                    \
        if (err)                                                  \
        {                                                         \
            return err;                                           \
        }                                                         \
                                                                  \
        err = des_receive_frame(context, &frame);                 \
        if (err)                                                  \
        {                                                         \
            return err;                                           \
        }                                                         \
                                                                  \
        if (frame.len != 1)                                       \
        {                                                         \
            return -1;                                            \
        }                                                         \
        output->value = frame.data[0];                            \
                                                                  \
        free(frame.data);                                         \
                                                                  \
        return DES_OK;                                            \
    }

#endif // MX_DES_FUNCTIONS_H