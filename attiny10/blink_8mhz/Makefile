# Include common makefile(s)
include ../attiny10.mk

# Set phony targets
.PHONY: all upload clean readfuses

# Set default goal
all: blink.hex

# Set up build goals
blink.hex: blink.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@
	# Print memory usage
	echo ""
	@avr-size -C --mcu=$(BOARD) $^

blink.elf: blink.o
	avr-gcc -g -mmcu=$(BOARD) -Xlinker -Map=$(basename $<).map -o $@ $^

blink.o: blink.cpp
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

# Phony goals
upload: blink.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

readfuses:
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U fuse:r:-:i

clean:
	$(RM) *.hex
	$(RM) *.elf
	$(RM) *.o
	$(RM) *.map