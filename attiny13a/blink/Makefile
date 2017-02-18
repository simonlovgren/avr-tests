PORT := /dev/ttyUSB0
BOARD := attiny13
PROGRAMMER := buspirate

.PHONY: all upload clean

all: blink.hex

upload: blink.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

blink.hex: blink.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

blink.elf: blink.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

blink.o: blink.cpp
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

clean:
	rm -f *.hex
	rm -f *.elf
	rm -f *.o
