#include <gba_console.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <stdio.h>
#include <stdlib.h>
#include <gba_interrupt.h>
#include <gba_input.h>

#include "gba_spi.h"

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
		iprintf("\x1b[%d;%dH Press \"A\" to send CMD0\n", 1, 0);
		iprintf("\x1b[%d;%dH Press \"B\" to toggle CS\n", 2, 0);
		iprintf("\x1b[%d;%dH Press \"UP\" to send ACMD41\n", 3, 0);

		VBlankIntrWait();
		scanKeys();
		u16 pressed = keysDown();
		if(pressed & KEY_A) {
			spi_set_cs(false);

			spi_transfer(0xFF);
			spi_transfer(0xFF);
			spi_transfer(0xFF);
			spi_transfer(0x40);
			spi_transfer(0x00);
			spi_transfer(0x00);
			spi_transfer(0x00);
			spi_transfer(0x00);
			spi_transfer(0x95);
			spi_transfer(0xFF);
			unsigned char resp = spi_transfer(0xFF);

			spi_set_cs(true);

			iprintf("\x1b[%d;%dH-Sent CMD0: %x", 5, 0, resp);

			delay(10);
		}else if(pressed & KEY_UP){
			// Send dummy
			spi_set_cs(false);
			spi_transfer(0xFF);
			spi_set_cs(true);

			delay(10);
		}else if(pressed & KEY_B){
			// Toggle CS
			spi_set_cs(!SPI_CS);
		}else if(pressed & KEY_RIGHT){
			// Clear screen
			iprintf("\x1b[2J");
		}
	}	
}


