#ifndef MAXON_DES_H
#define MAXON_DES_H

#include <stdint.h>

typedef struct
{
    int port;
} des_context;

typedef struct
{
    uint8_t opcode;
    uint8_t len;
    uint16_t *data;
} des_frame;

des_context *des_init(char *tty_port);

int des_enable(des_context *context);

int des_read_version(des_context *context);

#endif // MAXON_DES_H
