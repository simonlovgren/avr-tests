#ifndef F_CPU
#define F_CPU 8000000UL // Internal 8MHz clock
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PORTB-pins
  DDRB = 0x18;

  // Loop forever
  while(1) {
    // LED On/Off every other loop
    PORTB |= 0b00010000;
    PORTB &= 0b11110111;
    _delay_ms(250);
    PORTB |= 0b00001000;
    PORTB &= 0b11101111;
    _delay_ms(250);
  }
}
