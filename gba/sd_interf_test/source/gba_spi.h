#ifndef __GBA_SD_H
#define __GBA_SD_H

#define SPI_BASE 0xE00F000
#define SPI_DATA *(unsigned char volatile *)(SPI_BASE+0x00)
#define SPI_CTRL *(unsigned char volatile *)(SPI_BASE+0x01)

#define GBA_SD_BUSY  ((SPI_CTRL) & (1<<0)) // Start CMD transfer on set, clear when done
#define GBA_SD_CS    (volatile bool)((SPI_CTRL) & (1<<1)) // Set CS line high or low

#define GBA_SD_IDLE ((SPI_DATA) & (1<<0)) // Idle
#define GBA_SD_ERST ((SPI_DATA) & (1<<1)) // Erase Reset
#define GBA_SD_ILLC ((SPI_DATA) & (1<<2)) // Illegal Command
#define GBA_SD_CRCE ((SPI_DATA) & (1<<3)) // Command CRC Error
#define GBA_SD_SEQE ((SPI_DATA) & (1<<4)) // Erase Sequence Error
#define GBA_SD_ADDE ((SPI_DATA) & (1<<5)) // Address Error
#define GBA_SD_PARE ((SPI_DATA) & (1<<6)) // Parameter Error

#define SD_CMD_BASE 0x40
#define SD_CMD0   (SD_CMD_BASE + 0)
#define SD_CMD55  (SD_CMD_BASE + 55)
#define SD_CMD41  (SD_CMD_BASE + 41)
#define SD_ACMD41 (SD_CMD_BASE + 41)

// Written in assembly
bool spi_set_cs(bool);
volatile u8 spi_transfer(u8 data);

#endif
