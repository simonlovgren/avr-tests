# Include common makefile(s)
include ../atmega8.mk

# Set phony targets
.PHONY: all upload clean readfuses

# Set default goal
all: simplertc.hex

# Set up build goals
simplertc.hex: simplertc.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

simplertc.elf: simplertc.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

simplertc.o: simplertc.c
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

# Phony goals
upload: simplertc.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

readfuses:
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U efuse:r:-:i -U hfuse:r:-:i -U lfuse:r:-:i

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o