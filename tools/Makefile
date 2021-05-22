.PHONY: flash dump clean

TARGET=test_rom

flash: $(TARGET).rom
	dd if=$< of=/dev/sdg
	sync

%.rom: $(TARGET)/%.bin sdrom_build/sdrom_build
	sdrom_build/sdrom_build $< $@

$(TARGET)/%.bin:
	make -C $*

sdrom_build/sdrom_build:
	make -C sdrom_build

dump:
	make -C $(TARGET) dump

clean:
	rm -rf *.rom
	make -C $(TARGET) clean
	make -C sdrom_build clean
