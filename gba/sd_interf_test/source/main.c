#include <gba_console.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba_interrupt.h>
#include <gba_input.h>
#include "sd.h"

//#define WAITCNT (*((unsigned short volatile *) 0x4000204))
//#define ROM ((unsigned char volatile *) 0x8000000)
//#define SRAM ((unsigned char volatile *) 0xe000000)

void delay(int it){ for(;it;it--) VBlankIntrWait(); }

int main(void){
	irqInit();
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, scanKeys);

	consoleDemoInit();

	while(1){
		iprintf("\x1b[%d;%dH Press \"A\" to send CMD0\n", 0, 0);
		iprintf("\x1b[%d;%dH Press \"B\" to read resp\n", 1, 0);
		iprintf("\x1b[%d;%dH Press \"R\" to read resp\n", 2, 0);
		while(1){
			VBlankIntrWait();
			scanKeys();
			u16 pressed = keysDown();
			if(pressed & KEY_A) {
				//sd_reset();
				iprintf("\x1b[%d;%dH %x\n", 3, 0, sd_reset());
			} else if (pressed & KEY_B){
				iprintf("\x1b[%d;%dH %x\n", 3, 0, sd_read_resp());
			} else if (pressed & KEY_R){
				iprintf("\x1b[%d;%dH %x\n", 3, 0, sd_check_version());
			} else if (pressed & KEY_L){
				iprintf("\x1b[%d;%dH     \n", 3, 0);
			}
			delay(3);
		}

		//iprintf("Press \"A\" to check version: ");
		//while(1){
		//	VBlankIntrWait();
		//	scanKeys();
		//	u16 pressed = keysDown();
		//	if(pressed & KEY_A) break;
		//}
		//iprintf("%x\n", sd_check_version());
		//delay(10);
	}	
}


