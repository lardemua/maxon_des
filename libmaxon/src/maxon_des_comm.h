#ifndef MX_DES_COMM_H
#define MX_DES_COMM_H

#include <stdint.h>

#include "maxon_des.h"

typedef struct
{
    uint8_t opcode;
    uint8_t len;
    uint16_t *data;
} des_frame;

int des_init_comm(des_context *context, const char *portname);

void des_quit_comm(des_context *context);

des_error des_write_byte(des_context *context, uint8_t data);

des_error des_write_word(des_context *context, uint16_t data);

des_error des_write_data(des_context *context, uint16_t *data, int len);

des_error des_read_byte(des_context *context, uint8_t *data);

des_error des_read_word(des_context *context, uint16_t *data);

des_error des_read_data(des_context *context, uint16_t *data, int len);

des_error des_ack(des_context *context);

des_error des_send_frame(des_context *context, des_frame *frame);

des_error des_receive_frame(des_context *context, des_frame *frame);

#endif // MX_DES_COMM_H