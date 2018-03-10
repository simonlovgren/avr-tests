# Include common makefile(s)
include ../atmega328.mk

# Set phony targets
.PHONY: all upload clean

# Set default goal
all: doubleblink.hex

# Set up build goals
upload: doubleblink.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

doubleblink.hex: doubleblink.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

doubleblink.elf: doubleblink.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

doubleblink.o: doubleblink.cpp
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o
