#include "mx_des_crc.h"

#define P_CCITT 0x1021
static int crc_tabccitt_init = 0;
static unsigned short crc_tabccitt[256];

void des_init_crc()
{

    int i, j;
    uint16_t crc, c;

    for (i = 0; i < 256; i++)
    {
        crc = 0;
        c = ((uint16_t)i) << 8;

        for (j = 0; j < 8; j++)
        {

            if ((crc ^ c) & 0x8000)
                crc = (crc << 1) ^ P_CCITT;
            else
                crc = crc << 1;

            c = c << 1;
        }

        crc_tabccitt[i] = crc;
    }

    crc_tabccitt_init = 1;
}

uint16_t des_crc_update(uint16_t crc, uint8_t c)
{

    uint16_t tmp, short_c;

    short_c = 0x00ff & (uint16_t)c;

    tmp = (crc >> 8) ^ short_c;
    crc = (crc << 8) ^ crc_tabccitt[tmp];

    return crc;
}
