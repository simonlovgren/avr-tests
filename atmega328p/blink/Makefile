# Include common makefile(s)
include ../atmega328p.mk

# Set phony targets
.PHONY: all upload clean

# Set default goal
all: blink.hex

# Set up build goals
upload: blink.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

blink.hex: blink.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

blink.elf: blink.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

blink.o: blink.cpp
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o
