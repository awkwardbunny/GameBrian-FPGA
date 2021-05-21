#include <stdio.h>
#include <stdlib.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_input.h>

#include "gba_sd.h"
#include "timer.h"

#define WAITCNT (*((unsigned short volatile *) 0x4000204))
//#define ROM ((unsigned char volatile *) 0x8000000)
//#define SRAM ((unsigned char volatile *) 0xe000000)

void delay(int it){ for(;it;it--) VBlankIntrWait(); }

bool init(void){

	WAITCNT |= 0x0003; // SRAM waitstate = 8 cycles

	irqInit();
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, scanKeys);
	consoleDemoInit();

	timer_init();

	iprintf("Attempting to init sd...\n");
	spi_set_cs(true);
	bool sd_success;
	for(char i = 0; i < 10; i++){
		iprintf("Attempt #%d...\n", i);
		if(sd_success = init_sd()) break; // That's not a typo, it's supposed to be assignment
	}

	if(!sd_success){
		iprintf("ERR %x: %s\n", sd_errcmd, sd_strerr(sd_errno));
		return false;
	}

	iprintf("SD initialized successfully\n");
	iprintf("Version: ");
	switch(sd_version){
		case SD_VERSION_SD1:  iprintf("SD1\n"); break;
		case SD_VERSION_SD2:  iprintf("SD2\n"); break;
		case SD_VERSION_SDHC: iprintf("SDHC\n"); break;
		default:
		case SD_VERSION_UNK:  iprintf("UNKNOWN\n");
	}

	return true;
}

int main(void){
	init();

	while(1){
		u32 ms = millis();
		iprintf("\x1b[%d;%dH%d\n", 0, 29, (ms/1000) % 10);
		iprintf("\x1b[%d;%dH", 0, 0);

		VBlankIntrWait();
		scanKeys();
		u16 pressed = keysDown();
		if(pressed & KEY_START) {
			init();
			delay(5);
		}else if(pressed & KEY_A){
			iprintf("Reading from sd...");
			u8 *buf = malloc(16);
			if(read_sd(0, 0, 16, buf)){
				iprintf("S!\n");
				iprintf("DATA: %s\n", buf);
			} else {
				iprintf("F!\n");
				iprintf("ERR %x: %s\n", sd_errcmd, sd_strerr(sd_errno));
			}
			free(buf);

			delay(10);
		}else if(pressed & KEY_SELECT){
			// Clear screen
			iprintf("\x1b[2J");
			delay(10);
		}
	}	
}


