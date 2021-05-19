#include "gba_sd.h"

vu8 sd_send_cmd(u8 cmd, u32 args, u8 crc){
    spi_transfer(0xFF);
    spi_transfer(0xFF);
    spi_transfer(cmd);
    spi_transfer((args >> 24) & 0xFF);
    spi_transfer((args >> 16) & 0xFF);
    spi_transfer((args >>  8) & 0xFF);
    spi_transfer((args      ) & 0xFF);
    spi_transfer(crc);
    return spi_transfer(0xFF);
}

vu8 sd_send_acmd(u8 cmd, u32 args, u8 crc){
    sd_send_cmd(SD_CMD55, 0, 0xFF);
    return sd_send_cmd(cmd, args, crc);
}

vu8 sd_init(){
    return sd_send_cmd(SD_CMD0, 0, 0x95);
}