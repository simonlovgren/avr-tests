PORT := /dev/ttyUSB0
BOARD := attiny13
PROGRAMMER := buspirate


.PHONY: all upload

all: blink.hex

upload: blink.hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -p $(BOARD) -U flash:w:blink.hex:i

blink.hex: blink.elf
	avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex

blink.elf: blink.o
	avr-gcc -g -mmcu=attiny13 -o blink.elf blink.o

blink.o: blink.cpp
	avr-gcc -g -Os -mmcu=attiny13 -c blink.cpp

clean:
	rm -f blink.hex blink.elf blink.o
