#include "mx_des_comm.h"

#include <stdlib.h>
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

    if (port < 0)
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

    des_error err;

    uint8_t tmp[len * 2];
    for (int i = 0; i < len; i++)
    {
        tmp[i * 2] = data[i];
        tmp[i * 2 + 1] = data[i] >> 8;
    }

    int n = write(context->port, tmp, len * 2);
    if (n != len * 2)
    {
        return DES_COMM_ERROR;
    }

    return DES_OK;
}

des_error des_read_byte(des_context *context, uint8_t *data)
{
    assert(context != NULL);
    assert(data != NULL);

    for (int i = 0; i < 50; i++)
    {
        int n = read(context->port, data, 1);
        if (n == 1)
            return DES_OK;
        usleep(5000);
    }

    return DES_READ_TIMEOUT;
}

des_error des_read_word(des_context *context, uint16_t *data)
{
    assert(context != NULL);
    assert(data != NULL);

    int remaining = 2;

    uint8_t tmp[2];

    for (int i = 0; i < 50; i++)
    {
        int n = read(context->port, &tmp[2 - remaining], remaining);

        if (n == remaining)
        {
            *data = (tmp[0]) | (tmp[1] << 8);
            return DES_OK;
        }

        remaining -= n;

        usleep(5000);
    }

    return DES_READ_TIMEOUT;
}

des_error des_read_data(des_context *context, uint16_t *data, int len)
{
    assert(context != NULL);
    assert(data != NULL);
    assert(len >= 0);

    usleep(20000);
    uint8_t tmp[len * 2];

    int n = read(context->port, tmp, len * 2);
    if (n != len * 2)
        return DES_READ_ERROR;

    for (int i = 0; i < len; i++)
    {
        data[i] = (tmp[i * 2]) | (tmp[i * 2 + 1] << 8);
    }

    return DES_OK;
}

des_error des_ack(des_context *context)
{
    assert(context != NULL);

    uint8_t ack = 'a';

    des_error err = des_read_byte(context, &ack);
    if (err)
    {
        return err;
    }

    if (ack != 'O')
    {
        return ack == 'F'
                   ? DES_COMM_ACK_FAIL
                   : DES_COMM_ACK_UNDEFINED;
    }

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
        crc = des_crc_update(crc, (data >> 8) & 0xFF);
        crc = des_crc_update(crc, data & 0xFF);
    }

    return crc;
}

des_error des_send_frame(des_context *context, des_frame *frame)
{
    assert(context != NULL);
    assert(frame != NULL);

    // if the frame has len 0, append a dummy byte
    if (frame->len == 0)
    {
        frame->len = 1;
        frame->data = (uint16_t[1]){0x00};
    }
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

    err = des_write_data(context, frame->data, frame->len);
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

    frame->len += 1;
    // create the data buffer
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
    uint16_t ccrc = calculate_crc(frame);
    if (crc != ccrc)
    {
        des_write_byte(context, 'F');
        return DES_RECEIVE_BAD_CRC;
    }
    err = des_write_byte(context, 'O');
    if (err)
        return err;

    return DES_OK;
}
