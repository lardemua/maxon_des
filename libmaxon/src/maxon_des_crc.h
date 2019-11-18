#ifndef CRCCCITT_H
#define CRCCCITT_H

#include <stdint.h>

void des_init_crc();

uint16_t des_crc_update(uint16_t crc, uint8_t c);

#endif // CRCCCITT_H