.PHONY: all clean sd_interf_test code_from_sd

all: code_from_sd

code_from_sd:
	make -C $@
	scp $@/$@_mb.gba gbapi:
	ssh gbapi ./multiboot $@_mb.gba

sd_interf_test:
	make -C $@
	scp $@/$@_mb.gba gbapi:
	ssh gbapi ./multiboot $@_mb.gba

clean:
	make -C sd_interf_test clean
	make -C code_from_sd clean
