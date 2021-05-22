#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> 
#include "sdrom.h"

const char *usage = "Usage: %s input_rom_file output_file\n";

int main(int argc, char **argv){
	if(argc < 3){
		printf(usage, argv[0]);
		return -1;
	}

	char *rom_fn = argv[1];
	char *out_fn = argv[2];

	FILE *rom_f = fopen(rom_fn, "rb");
	if(!rom_f){
		printf("Error opening '%s': %s\n", rom_fn, strerror(errno));
		return -1;
	}

	FILE *out_f = fopen(out_fn, "wb");
	if(!out_f){
		printf("Error opening '%s': %s\n", out_fn, strerror(errno));
		return -1;
	}

	struct stat st;
	if(fstat(fileno(rom_f), &st)){
		printf("Fstat error: %s\n", strerror(errno));
		return -1;
	}

	printf("ROM File size (in bytes): %ld\n", st.st_size);

	printf("Writing header...");
	SDROM_hdr *header = (SDROM_hdr *)calloc(1, 512);
	header->magic = SDROM_MAGIC;
	header->version = SDROM_VER_01;
	header->type = SDROM_TYPE_GBA;
	strncpy(header->description, "Test GBA ROM", 32);
	header->description[31] = 0;
	header->start_block = 1;
	header->num_blocks = ((st.st_size/512) + 1);
	header->entry = 0;

	int written = fwrite(header, 512, 1, out_f);
	if(!written){
		printf("Write error: %s\n", strerror(errno));
		return -1;
	}
	printf("Done!\n");


	printf("Writing ROM...");
	int read = 0;
	char readBuf[512];
	for(int i = 0; i < (st.st_size/512); i++){
		read = fread(readBuf, 512, 1, rom_f);
		if(!read){
			printf("Read error: %s\n", strerror(errno));
			return -1;
		}

		written = fwrite(readBuf, 512, 1, out_f);
		if(!written){
			printf("Write error: %s\n", strerror(errno));
			return -1;
		}
	}

	// Read the last partial block
	read = fread(readBuf, st.st_size % 512, 1, rom_f);
	if(!read){
		printf("Read error: %s\n", strerror(errno));
		return -1;
	}

	written = fwrite(readBuf, st.st_size % 512, 1, out_f);
	if(!written){
		printf("Write error: %s\n", strerror(errno));
		return -1;
	}
	printf("Done!\n");

	// Not too concerned about not closing when error
	fclose(rom_f);
	fclose(out_f);
	return 0;
}
