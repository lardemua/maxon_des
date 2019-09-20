#ifndef CRCCCITT_H
#define CRCCCITT_H

#include <stdint.h>

void init_crcccitt();

uint16_t update_crcccitt(uint16_t crc, uint8_t c);

#endif // CRCCCITT_H