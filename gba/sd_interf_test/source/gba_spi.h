#ifndef __GBA_SPI_H
#define __GBA_SPI_H

#include <gba_types.h>

#define SPI_BASE 0xE00F000
#define SPI_DATA *(vu8 *)(SPI_BASE+0x00)
#define SPI_CTRL *(vu8 *)(SPI_BASE+0x01)

#define SPI_BUSY  ((SPI_CTRL) & (1<<0)) // Start CMD transfer on set, clear when done
#define SPI_CS    (volatile bool)((SPI_CTRL) & (1<<1)) // Set CS line high or low


// Written in assembly
bool spi_set_cs(bool);
vu8 spi_transfer(u8 data);

#endif
