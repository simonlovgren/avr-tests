# Include common makefile(s)
include ../attiny85.mk

# Set phony targets
.PHONY: all upload clean

# Set default goal
all: captouch.hex

# Set up build goals
upload: captouch.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

captouch.hex: captouch.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

captouch.elf: captouch.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

captouch.o: captouch.cpp
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o
