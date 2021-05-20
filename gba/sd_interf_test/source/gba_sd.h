#ifndef __GBA_SD_H
#define __GBA_SD_H

#include <gba_types.h>
#include "gba_spi.h"

#define SD_R1_IDLE                 (1<<0)
#define SD_R1_ERASE_RESET          (1<<1)
#define SD_R1_ILLEGAL_COMMAND      (1<<2)
#define SD_R1_CRC_ERROR            (1<<3)
#define SD_R1_ERASE_SEQUENCE_ERROR (1<<4)
#define SD_R1_ADDRESS_ERROR        (1<<5)
#define SD_R1_PARAMETER_ERROR      (1<<6)

#define SD_VERSION1    0x01
#define SD_VERSION2    0x02
#define SD_VERSION2HC  0x03
#define SD_VERSION_ERR 0xFF

#define SD_CMD_BASE 0x40
#define SD_CMD0   (SD_CMD_BASE + 0)
#define SD_CMD8   (SD_CMD_BASE + 8)
#define SD_CMD55  (SD_CMD_BASE + 55)
#define SD_CMD58  (SD_CMD_BASE + 58)
#define SD_CMD41  (SD_CMD_BASE + 41)
#define SD_ACMD41 (SD_CMD_BASE + 41)

#define SD_ERR_NONE           0x00
#define SD_ERR_ILLEGAL        0x01
#define SD_ERR_CRC            0x02
#define SD_ERR_ERASE_SEQUENCE 0x03
#define SD_ERR_ADDRESS        0x04
#define SD_ERR_PARAMETER      0x05
#define SD_ERR_BLANK          0x06
#define SD_ERR_ERASE_RESET    0x07

extern int sd_errno;

bool init_sd(void);

u8 sd_send_cmd(u8 cmd, u32 args, u8 crc);
u8 sd_send_acmd(u8 cmd, u32 args, u8 crc);

void sd_dummy(void);
u8 sd_set_errno(u8 resp);
const char* sd_strerr(int errno);

u8 sd_reset(void);
u8 sd_version(void);
u8 sd_init(u8 version);
u32 sd_ocr(void);

#endif
