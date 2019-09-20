#include "mx_des.h"

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termio.h>
#include <fcntl.h>

#include "crcccitt.h"
#include "mx_des_functions.h"

int _write_msg_inbuf(des_context *context, uint8_t *data, int len)
{
    int n = write(context->port, data, len);

    if (n < 0 || n != len)
    {
        printf("error: write_msg_inbuf: size == %d\n", n);
        return -1;
    }

    return 0;
}

int _send_opcode(des_context *context, uint8_t opcode)
{
    int err = _write_msg_inbuf(context, &opcode, 1);
    if (err != 0)
    {
        printf("error: _send_opcode: err == %d\n", err);
        return -1;
    }
}

int _check_handshake(des_context *context)
{
    uint8_t bytes;
    usleep(100000);
    int n = read(context->port, &bytes, 1);
    if (n != 1)
    {
        printf("error: _check_handshake: n == %d\n", n);
        return -1;
    }

    if (bytes != 'O')
    {
        printf("error: _check_handshake: byte == %c\n", bytes);
        return -1;
    }

    return 0;
}

des_context *des_init(char *tty_port)
{
    init_crcccitt();

    // Opens the serial port to the DES
    int port = open(tty_port, O_RDWR | O_NONBLOCK);
    tcflush(port, TCIFLUSH);

    if (port == -1)
    {
        // TODO: add error handling
        return NULL;
    }

    struct termios params = {
        .c_cflag = B38400 | CS8 | CLOCAL | CREAD,
        .c_iflag = IGNPAR,
        .c_oflag = 0};

    int err = tcsetattr(port, TCSANOW, &params);
    if (err < 0)
    {
        // TODO: add error handling
        return NULL;
    }

    des_context *context = malloc(sizeof(des_context));
    context->port = port;

    return context;
}

int des_enable(des_context *context)
{
    int err;

    err = _send_opcode(context, 0x05);
    if (err)
    {
        printf("error: des_enable_version: error sending opcode\n");
        return err;
    }

    usleep(50000);
    tcflush(context->port, TCIFLUSH);
    err = _check_handshake(context);
    if (err)
    {
        printf("error: des_enable_version: error checking handshake\n");
        return err;
    }

    uint8_t data;

    data = 0;
    err += _write_msg_inbuf(context, &data, 1);
    data = 1;
    err += _write_msg_inbuf(context, &data, 1);
    data = 0;
    err += _write_msg_inbuf(context, &data, 1);
    if (err)
        return -1;

    // Calculate crc
    uint16_t crc = 0x00;
    crc = update_crcccitt(crc, 0x05);
    crc = update_crcccitt(crc, 0x00);
    crc = update_crcccitt(crc, 0x01);
    crc = update_crcccitt(crc, 0x00);
    printf("crc = %x\n", crc);

    // SEND CRC
    data = crc;
    err += _write_msg_inbuf(context, &data, 1);
    data = (crc >> 8);
    err += _write_msg_inbuf(context, &data, 1);

    if (err)
    {
        printf("error sending data\n");
        return -1;
    }

    tcflush(context->port, TCIFLUSH);
    err = _check_handshake(context);
    if (err)
    {
        printf("error: des_enable_version: error checking handshake after data\n");
        return err;
    }
}

int des_read_version(des_context *context)
{
    int err;

    err = _send_opcode(context, 0x1A);
    if (err)
    {
        printf("error: des_read_version: error sending opcode\n");
        return err;
    }

    tcflush(context->port, TCIFLUSH);
    err = _check_handshake(context);
    if (err)
    {
        printf("error: des_read_version: error checking handshake on opcode\n");
        return err;
    }

    uint8_t data;
    // SEND LEN
    data = 0;
    err += _write_msg_inbuf(context, &data, 1);
    // SEND DATA
    data = 0;
    err += _write_msg_inbuf(context, &data, 1);
    data = 0;
    err += _write_msg_inbuf(context, &data, 1);

    // Calculate crc
    uint16_t crc = 0x00;
    crc = update_crcccitt(crc, 0x1A);
    crc = update_crcccitt(crc, 0x00);
    crc = update_crcccitt(crc, 0x00);
    crc = update_crcccitt(crc, 0x00);
    printf("crc = %x\n", crc);

    // SEND CRC
    data = crc;
    err += _write_msg_inbuf(context, &data, 1);
    data = (crc >> 8);
    err += _write_msg_inbuf(context, &data, 1);

    tcflush(context->port, TCIFLUSH);
    err = _check_handshake(context);
    if (err)
    {
        printf("error: des_read_version: error checking handshake after data\n");
        return err;
    }

    // RECEIVE DATA
    usleep(50000);
    int size = read(context->port, &data, 1);
    if (size != 1)
    {
        printf("error: des_read_version: no response opcode received\n");
        return -1;
    }

    if (data != 0x00)
    {
        printf("error: des_read_version: error getting response opcode\n");
        return -1;
    }

    data = 'O';
    err = _write_msg_inbuf(context, &data, 1);
    if (err)
    {
        printf("error: des_read_version: error sending acknowledge\n");
        return -1;
    }

    usleep(100000);
    size = read(context->port, &data, 1);
    if (size != 1)
    {
        printf("error: des_read_version: no response data\n");
        return -1;
    }

    printf("len data received: %d\n", data);

    uint8_t received[10];
    usleep(100000);
    size = read(context->port, &received, 10);
    if (size != 10)
    {
        printf("error: des_read_version: bytes received = %d\n", size);
        return -1;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("data[%d] = %x\n", i, received[i]);
    }
    return 0;
}