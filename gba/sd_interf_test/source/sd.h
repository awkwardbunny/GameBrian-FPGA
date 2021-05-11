#ifndef __GBA_SD_H
#define __GBA_SD_H

#define SD_BASE ((unsigned char volatile *) 0xE00F000)
//#define SD_CMD  *(SD_BASE + 0x000)
#define SD_CMD  *((unsigned char volatile *) 0xE00F000)
#define SD_ARG0 *((unsigned char volatile *) 0xE00F001)
#define SD_ARG1 *((unsigned char volatile *) 0xE00F002)
#define SD_ARG2 *((unsigned char volatile *) 0xE00F003)
#define SD_ARG3 *((unsigned char volatile *) 0xE00F004)
#define SD_CRC  *((unsigned char volatile *) 0xE00F005)
#define SD_NUMH *((unsigned char volatile *) 0xE00F006)
#define SD_NUML *((unsigned char volatile *) 0xE00F007)
#define SD_RESP *((unsigned char volatile *) 0xE00F008)
#define SD_STAT *((unsigned char volatile *) 0xE00F009)

volatile u8 sd_send_cmd(u8 cmd, u8 a0, u8 a1, u8 a2, u8 a3, u8 crc, u16 num){
	SD_CMD = cmd;
	SD_ARG0 = a0;
	SD_ARG1 = a1;
	SD_ARG2 = a2;
	SD_ARG3 = a3;
	SD_CRC = crc;
	SD_NUML = (num & 0xFF);
	SD_NUMH = (num >> 8);
	SD_STAT = 0x01;

	while(SD_STAT & 0x01);
	return SD_RESP;
}

volatile u8 sd_read_resp(){
	return SD_RESP;
}

volatile u8 sd_reset(){
	return sd_send_cmd(0x40, 0, 0, 0, 0, 0x95, 0);
}

volatile u8 sd_check_version(){
	return sd_send_cmd(0x48, 0, 0, 0x01, 0xAA, 0x87, 0);
}

#endif
