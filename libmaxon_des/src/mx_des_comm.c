#include "mx_des_comm.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "mx_des_crc.h"

des_error des_init_comm(des_context *context, const char *portname)
{
    assert(context != NULL);
    assert(portname != NULL);

    int port = open(portname, O_RDWR | O_NONBLOCK);
    tcflush(port, TCIFLUSH);

    if (port)
    {
        return DES_COMM_INIT_ERROR;
    }

    struct termios params = {
        .c_cflag = B38400 | CS8 | CLOCAL | CREAD,
        .c_iflag = IGNPAR,
        .c_oflag = 0};

    int err = tcsetattr(port, TCSANOW, &params);
    if (err)
    {
        return DES_COMM_INIT_ERROR;
    }

    context->port = port;

    return DES_OK;
}

des_error des_write_byte(des_context *context, uint8_t data)
{
    assert(context != NULL);

    int n = write(context->port, &data, 1);
    if (n != 1)
    {
        return DES_COMM_ERROR;
    }

    return DES_OK;
}

des_error des_write_word(des_context *context, uint16_t data)
{
    assert(context != NULL);

    uint8_t tmp[2] = {
        data & 0xFF,
        (data >> 8) & 0xFF,
    };

    int n = write(context->port, tmp, 2);
    if (n != 2)
    {
        return DES_COMM_ERROR;
    }

    return DES_OK;
}

des_error des_write_data(des_context *context, uint16_t *data, int len)
{
    assert(context != NULL);
    assert(data != NULL);
    assert(len >= 0);

    int err = DES_OK;

    for (int i = 0; i < len; i++)
    {
        err = des_write_word(context, data[i]);
    }

    return err;
}

des_error des_read_byte(des_context *context, uint8_t *data)
{
    assert(context != NULL);
    assert(data != NULL);

    int n = read(context->port, &data, 1);

    if (n != 1)
    {
        return DES_COMM_ERROR;
    }

    return DES_OK;
}

des_error des_read_word(des_context *context, uint16_t *data)
{
    assert(context != NULL);
    assert(data != NULL);

    uint8_t tmp[2];

    int n = read(context->port, &tmp, 2);

    if (n != 2)
    {
        return DES_COMM_ERROR;
    }

    *data = (tmp[0] << 8) | tmp[1];

    return DES_OK;
}

des_error des_read_data(des_context *context, uint16_t *data, int len)
{
    assert(context != NULL);
    assert(data != NULL);
    assert(len >= 0);

    for (int i = 0; i < len; i++)
    {
        des_error err = des_read_word(context, &data[i]);
        if (err)
            return err;
    }

    return DES_OK;
}

des_error des_ack(des_context *context)
{
    assert(context != NULL);

    uint8_t ack = 0;

    des_error err = des_read_byte(context, &ack);

    if (ack != 'O')
        return ack == 'F'
                   ? DES_COMM_ACK_FAIL
                   : DES_COMM_ACK_UNDEFINED;

    return DES_OK;
}

uint16_t calculate_crc(des_frame *frame)
{
    assert(frame != NULL);

    uint16_t crc = 0;

    crc = des_crc_update(crc, frame->opcode);
    crc = des_crc_update(crc, frame->len - 1);

    for (int i = 0; i < frame->len; i++)
    {
        uint16_t data = frame->data[i];
        crc = des_crc_update(crc, data & 0xFF);
        crc = des_crc_update(crc, (data >> 8) & 0xFF);
    }

    return crc;
}

des_error des_send_frame(des_context *context, des_frame *frame)
{
    assert(context != NULL);
    assert(frame != NULL);

    uint16_t crc = calculate_crc(frame);

    des_error err = DES_OK;

    // send the opcode
    err = des_write_byte(context, frame->opcode);
    if (err)
        return err;
    // acknowledge the opcode reception
    err = des_ack(context);
    if (err)
        return err;
    // send the len - 1
    err = des_write_byte(context, frame->len - 1);
    if (err)
        return err;
    // send the data
    if (frame->len == 0)
    {
        err = des_write_data(context, frame->data, frame->len);
    }
    else
    {
        // write a dummy byte
        err = des_write_word(context, 0);
    }
    if (err)
        return err;
    // send the crc
    err = des_write_word(context, crc);
    if (err)
        return err;
    // acknowledge the data reception
    err = des_ack(context);
    if (err)
        return err;

    return DES_OK;
}

des_error des_receive_frame(des_context *context, des_frame *frame)
{
    assert(context != NULL);
    assert(frame != NULL);

    des_error err = DES_OK;

    // receive the opcode (has to be 0x00)
    uint8_t b;
    err = des_read_byte(context, &frame->opcode);
    if (err)
        return err;
    if (frame->opcode != 0x00)
        return DES_RECEIVE_BAD_OPCODE;
    // send the acknoledge
    err = des_write_byte(context, 'O');
    if (err)
        return err;
    // read the len-1
    err = des_read_byte(context, &frame->len);
    if (err)
        return err;
    // increase the len by one, because what we receive is (len-1)
    frame->len += 1;
    // create the data buffer
    assert(frame->len < 50);
    frame->data = malloc(sizeof(uint16_t) * frame->len);
    memset(frame->data, 0, frame->len);
    // receive the data
    err = des_read_data(context, frame->data, frame->len);
    if (err)
        return err;
    // check the crc and acknoledge
    uint16_t crc;
    err = des_read_word(context, &crc);
    if (err)
        return err;
    if (crc != calculate_crc(frame))
    {
        des_write_byte(context, 'F');
        return DES_RECEIVE_BAD_CRC;
    }
    err = des_write_byte(context, 'O');
    if (err)
        return err;

    return DES_OK;
}
