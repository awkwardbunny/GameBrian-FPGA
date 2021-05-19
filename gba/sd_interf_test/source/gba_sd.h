#ifndef __GBA_SD_H
#define __GBA_SD_H

#include <gba_types.h>
#include "gba_spi.h"

#define SD_RESP_IDLE ((SPI_DATA) & (1<<0)) // Idle
#define SD_RESP_ERST ((SPI_DATA) & (1<<1)) // Erase Reset
#define SD_RESP_ILLC ((SPI_DATA) & (1<<2)) // Illegal Command
#define SD_RESP_CRCE ((SPI_DATA) & (1<<3)) // Command CRC Error
#define SD_RESP_SEQE ((SPI_DATA) & (1<<4)) // Erase Sequence Error
#define SD_RESP_ADDE ((SPI_DATA) & (1<<5)) // Address Error
#define SD_RESP_PARE ((SPI_DATA) & (1<<6)) // Parameter Error

#define SD_CMD_BASE 0x40
#define SD_CMD0   (SD_CMD_BASE + 0)
#define SD_CMD55  (SD_CMD_BASE + 55)
#define SD_CMD41  (SD_CMD_BASE + 41)
#define SD_ACMD41 (SD_CMD_BASE + 41)

volatile u8 sd_send_cmd(u8 cmd, u32 args, u8 crc);
volatile u8 sd_send_acmd(u8 cmd, u32 args, u8 crc);
volatile u8 sd_init();

#endif
