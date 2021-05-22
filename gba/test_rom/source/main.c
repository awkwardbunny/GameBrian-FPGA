#include <stdio.h>
#include <stdlib.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_input.h>

void delay(int it){ for(;it;it--) VBlankIntrWait(); }

int main(void){
	irqInit();
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, scanKeys);
	consoleDemoInit();
	
	while(1){
		iprintf("A\n");

		VBlankIntrWait();
		scanKeys();
		u16 pressed = keysDown();
		if(pressed & KEY_START) {
			iprintf("START\n");
		}
		delay(10);
	}	
}


