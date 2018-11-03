# Include common makefile(s)
include ../atmega8.mk

# Set phony targets
.PHONY: all upload clean readfuses

# Set default goal
all: doubleblink.hex

# Set up build goals
doubleblink.hex: doubleblink.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

doubleblink.elf: doubleblink.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

doubleblink.o: doubleblink.cpp
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

# Phony goals
upload: doubleblink.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

readfuses:
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U efuse:r:-:i -U hfuse:r:-:i -U lfuse:r:-:i

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o
