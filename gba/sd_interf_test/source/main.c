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

	while(1){
		iprintf("\x1b[%d;%dH CS: %x\n", 0, 0, SPI_CS);

		VBlankIntrWait();
		scanKeys();
		u16 pressed = keysDown();
		if(pressed & KEY_START) {

			bool init_success = init_sd();
			iprintf("Init SD is %s\n", init_success ? "successful" : "failed");
			iprintf("Version: ");
			switch(sd_version){
				case SD_VERSION_SD1:  iprintf("SD1\n"); break;
				case SD_VERSION_SD2:  iprintf("SD2\n"); break;
				case SD_VERSION_SDHC: iprintf("SDHC\n"); break;
				default:
				case SD_VERSION_UNK:  iprintf("UNKNOWN\n");
			}
			
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


