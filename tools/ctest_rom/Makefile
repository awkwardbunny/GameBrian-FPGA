.PHONY: clean dump

TARGET=$(shell basename $(CURDIR))

all: $(TARGET).bin

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@

%.elf: %.c
	arm-none-eabi-gcc -nostdlib -ffreestanding -mcpu=arm7tdmi -mthumb -mthumb-interwork $^ -o $@

dump: $(TARGET).elf
	arm-none-eabi-objdump -d $^

clean:
	rm -f *.bin *.elf
