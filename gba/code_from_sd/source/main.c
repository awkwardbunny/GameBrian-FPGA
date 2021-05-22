#include <stdio.h>
#include <stdlib.h>
#include <gba_console.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>
#include <gba_input.h>

#include "gba_sd.h"
#include "timer.h"
#include "sdrom.h"

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

u8 *romBuf = NULL;

int main(void){
	init();

	SDROM_hdr *hdr = (SDROM_hdr *)malloc(512);
	
	while(1){
		u32 ms = millis();
		iprintf("\x1b[%d;%dH%ld\n", 0, 29, (ms/1000) % 10);
		iprintf("\x1b[%d;%dH", 0, 0);

		VBlankIntrWait();
		scanKeys();
		u16 pressed = keysDown();
		if(pressed & KEY_START) {
			init();
			delay(5);
		}else if(pressed & KEY_A){
			iprintf("Reading header from sd...");
			if(read_sd(0, 0, 512, (u8 *)hdr)){
				iprintf("S!\n");
			} else {
				iprintf("F!\n");
				iprintf("ERR %x: %s\n", sd_errcmd, sd_strerr(sd_errno));
			}

			delay(10);
		}else if(pressed & KEY_B){
			iprintf("Parsing header...\n");
			if(hdr->magic != SDROM_MAGIC){
				iprintf("E: Invalid magic!\n");
				iprintf("Expected: %x\n", SDROM_MAGIC);
				iprintf("Got:      %lx\n", hdr->magic);
				continue;
			}
			iprintf("Version: %ld\n", hdr->version);
			iprintf("Type: %d\n", hdr->type);
			iprintf("Description: %s\n", hdr->description);
			iprintf("Start: %lx\n", hdr->start_block);
			iprintf("Num: %lx\n", hdr->num_blocks);
			iprintf("Entry: %lx\n", hdr->entry);

			iprintf("\nMalloc'd addr: %p\n", (void *)hdr);
		}else if(pressed & KEY_UP){
			romBuf = malloc(hdr->num_blocks * 512);
			iprintf("\nMalloc'd addr: %p\n", (void *)romBuf);

			for(int i = 0; i < hdr->num_blocks; i++){
				iprintf("Reading block #%ld\n", i);
				if(!read_sd(i+hdr->start_block, 0, 512, romBuf + (i*512))){
					iprintf("ERR %x: %s\n", sd_errcmd, sd_strerr(sd_errno));
				}
			}

		}else if(pressed & KEY_DOWN){
			free(romBuf);
		}else if(pressed & KEY_LEFT){
			iprintf("%x %x %x %x   %x %x %x %x\n",
				romBuf[3], romBuf[2], romBuf[1], romBuf[0],
				romBuf[7], romBuf[6], romBuf[5], romBuf[4]
			);
		}else if(pressed & KEY_RIGHT){
			iprintf("Jumping to code!\n");
			void *code = romBuf + hdr->entry;
			iprintf("%x\n", ((u8(*)())code)());
		}else if(pressed & KEY_SELECT){
			// Clear screen
			iprintf("\x1b[2J");
			delay(10);
		}
	}	
}


