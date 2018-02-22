# Include common makefile(s)
include ../atmega328.mk

# Set phony targets
.PHONY: all upload clean

# Set default goal
all: simplertc.hex

# Set up build goals
upload: simplertc.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

simplertc.hex: simplertc.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

simplertc.elf: simplertc.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

simplertc.o: simplertc.c
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o
