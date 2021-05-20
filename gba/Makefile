.PHONY: all clean sd_interf_test

all: sd_interf_test

sd_interf_test:
	make -C $@
	scp $@/$@_mb.gba gbapi:
	ssh gbapi ./multiboot $@_mb.gba

clean:
	make -C sd_interf_test clean
