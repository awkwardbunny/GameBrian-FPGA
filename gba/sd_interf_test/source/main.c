#include <stdio.h>
#include <stdlib.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_input.h>

#include "gba_sd.h"

#define WAITCNT (*((unsigned short volatile *) 0x4000204))
//#define ROM ((unsigned char volatile *) 0x8000000)
//#define SRAM ((unsigned char volatile *) 0xe000000)

void delay(int it){ for(;it;it--) VBlankIntrWait(); }

int main(void){
	irqInit();
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, scanKeys);

	consoleDemoInit();

	WAITCNT |= 0x0003; // SRAM waitstate = 8 cycles

	spi_set_cs(true);
	u8 ver = SD_VERSION_ERR;

	while(1){
		iprintf("\x1b[%d;%dH CS: %x\n", 0, 0, SPI_CS);

		VBlankIntrWait();
		scanKeys();
		u16 pressed = keysDown();
		if(pressed & KEY_START) {

			sd_reset();
			iprintf("\x1b[%d;%dH-CMD0: %s\n", 2, 0, sd_strerr(sd_errno));
			
			delay(10);
		}else if(pressed & KEY_A){
			iprintf("\x1b[%d;%dH-CMD8: ", 3, 0);
			
			ver = sd_version();
			if(ver == SD_VERSION1)
				iprintf("SD1\n");
			else if(ver == SD_VERSION2)
				iprintf("SD2\n");
			else
				iprintf("ERROR\n");

			delay(10);
		}else if(pressed & KEY_B){
			iprintf("\x1b[%d;%dH-ACMD41: ", 4, 0);
			
			u8 resp = sd_init(ver);
			if(resp & SD_R1_IDLE)
				iprintf("IDLE\n");
			else
				iprintf("%s\n", sd_strerr(sd_errno));
			
			delay(10);
		}else if(pressed & KEY_UP){
			iprintf("\x1b[%d;%dH-CMD58: ", 5, 0);
			
			u32 ocr = sd_ocr();
			if(sd_errno)
				iprintf("%s\n", sd_strerr(sd_errno));
			else
				iprintf("%lx\n", ocr);

			delay(10);
		}else if(pressed & KEY_L){
			// Toggle CS
			spi_set_cs(!SPI_CS);
			delay(10);
		}else if(pressed & KEY_R){
			// Send dummy
			sd_dummy();
			delay(10);
		}else if(pressed & KEY_SELECT){
			// Clear screen
			iprintf("\x1b[2J");
			delay(10);
		}
	}	
}


