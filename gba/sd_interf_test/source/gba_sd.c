#include "gba_sd.h"

int sd_errcmd = SD_CMD0;
int sd_errno = SD_ERR_NONE;
int sd_version = SD_VERSION_UNK;

u8 sd_send_cmd(u8 cmd, u32 args, u8 crc){
    spi_transfer(0xFF); // DUMMY
    spi_transfer(0xFF); // DUMMY
    spi_transfer(cmd);
    spi_transfer((args >> 24) & 0xFF);
    spi_transfer((args >> 16) & 0xFF);
    spi_transfer((args >>  8) & 0xFF);
    spi_transfer((args      ) & 0xFF);
    spi_transfer(crc);

    u8 resp;
    u32 timeout = millis() + SD_CMD_TIMEOUT;
    do {
        resp = sd_set_errno(spi_transfer(0xFF));
    } while ((resp & 0x80) && (millis() < timeout));

    if(millis() > timeout){
        sd_errno = SD_ERR_TIMEOUT;
    }

    sd_errcmd = cmd;
    return resp;
}

u8 sd_send_acmd(u8 cmd, u32 args, u8 crc){
    u8 resp = sd_send_cmd(SD_CMD55, 0, 0xFF);
    if(sd_errno)
        return resp;

    return sd_send_cmd(cmd, args, crc);
}

u8 sd_set_errno(u8 resp){
    if(resp == 0xFF) sd_errno = SD_ERR_BLANK;
    else if(resp & SD_R1_ILLEGAL_COMMAND) sd_errno = SD_ERR_ILLEGAL;
    else if(resp & SD_R1_CRC_ERROR) sd_errno = SD_ERR_CRC;
    else if(resp & SD_R1_ERASE_SEQUENCE_ERROR) sd_errno = SD_ERR_ERASE_SEQUENCE;
    else if(resp & SD_R1_ADDRESS_ERROR) sd_errno = SD_ERR_ADDRESS;
    else if(resp & SD_R1_PARAMETER_ERROR) sd_errno = SD_ERR_PARAMETER;
    else if(resp & SD_R1_ERASE_RESET) sd_errno = SD_ERR_ERASE_RESET;
    else sd_errno = SD_ERR_NONE;
    return resp;
}

const char* sd_strerr(int errno){
    switch(sd_errno){
        case SD_ERR_NONE:           return "SUCCESS";
        case SD_ERR_ILLEGAL:        return "ILLEGAL COMMAND";
        case SD_ERR_CRC:            return "CRC ERROR";
        case SD_ERR_ERASE_SEQUENCE: return "ERASE SEQ ERROR";
        case SD_ERR_ADDRESS:        return "ADDRESS ERROR";
        case SD_ERR_PARAMETER:      return "PARAMETER ERROR";
        case SD_ERR_BLANK:          return "EMPTY RESPONSE";
        case SD_ERR_ERASE_RESET:    return "ERASE RESET";
        case SD_ERR_VERSION:        return "VERSION ERROR";
        case SD_ERR_TIMEOUT:        return "TIMEOUT";
        default:                    return "UNKNOWN ERROR";
    }
}

void sd_dummy(){
    spi_set_cs(false);
    spi_transfer(0xFF);
    spi_set_cs(true);
}

u8 sd_reset(){
    spi_set_cs(false);
    u8 resp = sd_send_cmd(SD_CMD0, 0, 0x95);
    spi_set_cs(true);

    return resp;
}

u8 sd_get_version(){
    spi_set_cs(false);
    u8 resp = sd_send_cmd(SD_CMD8, 0x1AA, 0x87);

    if(sd_errno == SD_ERR_ILLEGAL){
        sd_version = SD_VERSION_SD1;
    } else if(sd_errno == SD_ERR_NONE){
        spi_transfer(0xFF);
        spi_transfer(0xFF);
        spi_transfer(0xFF);
        if(spi_transfer(0xFF) == 0xAA){
            sd_version = SD_VERSION_SD2;
        } else {
            sd_errno = SD_ERR_VERSION;
        }
    }
    spi_transfer(0xFF);
    spi_set_cs(true);

    return resp;
}

u8 sd_init(){
    u32 arg = (sd_version & SD_VERSION_SD2) ? 0x40000000 : 0;

    spi_set_cs(false);
    u8 resp = sd_send_acmd(SD_ACMD41, arg, 0xFF);

    spi_transfer(0xFF);
    spi_set_cs(true);

    return resp;
}

u32 sd_ocr(){
    u32 ocr = 0x00;

    spi_set_cs(false);
    sd_send_cmd(SD_CMD58, 0x0, 0xFF);
    if(sd_errno == SD_ERR_NONE){
        ocr |= (spi_transfer(0xFF) << 24);
        ocr |= (spi_transfer(0xFF) << 16);
        ocr |= (spi_transfer(0xFF) <<  8);
        ocr |= (spi_transfer(0xFF)      );
    } else {
        ocr = 0xFFFFFFFF;
    }

    spi_transfer(0xFF);
    spi_set_cs(true);

    return ocr;
}

bool init_sd(){
	sd_reset();
	if(sd_errno)
		return false;

	u8 resp = sd_get_version();
	if(sd_errno)
		return false;

    u16 timeout = millis() + SD_INIT_TIMEOUT;
	do {
		resp = sd_init();
		if(sd_errno)
			return false;
	} while((resp & SD_R1_IDLE) && (millis() < timeout));

    if(millis() > timeout){
        sd_errno = SD_ERR_TIMEOUT;
        return false;
    }

	u32 ocr = sd_ocr();
	if(sd_errno)
		return false;

    if(ocr >> 24 == 0xC0){
        sd_version = SD_VERSION_SDHC;
    }

	return true;
}

bool read_sd(u32 block, u16 offset, u16 count, u8 *dest){
    if(!count)
        return true;

    if(count + offset > 512)
        return false;

    if(sd_version != SD_VERSION_SDHC){
        block <<= 9;
    }

    spi_set_cs(false);
    sd_send_cmd(SD_CMD17, block, 0xFF);
    spi_transfer(0xFF);
    if(sd_errno){
        spi_set_cs(true);
        return false;
    }

    u16 timeout = millis() + SD_READ_TIMEOUT;

    // Wait until data token
    // TODO?: It should actually be 0xFE, but don't know if that makes a difference
    while((spi_transfer(0xFF) == 0xFF) && (millis() < timeout));

    if(millis() > timeout){
        spi_set_cs(true);
        sd_errcmd = 0xFF;
        sd_errno = SD_ERR_TIMEOUT;
        return false;
    }

    u16 trailing = 512 - offset - count;

    // Skip offset bytes
    for(;offset;offset--){
        spi_transfer(0xFF);
    }

    // Read bytes
    for(int i = 0; i < count; i++){
        // iprintf("%x ", spi_transfer(0xFF));
        dest[i] = spi_transfer(0xFF);
    }

    // Ignore trailing bytes
    for(;trailing;trailing--){
        spi_transfer(0xFF);
    }

    // CRC?
    // TODO: Check CRC
    spi_transfer(0xFF);
    spi_transfer(0xFF);

    // Trailing dummy byte
    spi_transfer(0xFF);
    spi_set_cs(true);
    return true;
}