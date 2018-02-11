PORT := /dev/ttyUSB0
BOARD := atmega328
PROGRAMMER := buspirate

.PHONY: all upload clean

all: simplertc.hex

upload: simplertc.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:$^:i

simplertc.hex: simplertc.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

simplertc.elf: simplertc.o
	avr-gcc -g -mmcu=$(BOARD) -o $@ $^

simplertc.o: simplertc.c
	avr-gcc -g -Os -mmcu=$(BOARD) -c $^

clean:
	rm -f *.hex
	rm -f *.elf
	rm -f *.o
