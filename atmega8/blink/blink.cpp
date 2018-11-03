#ifndef F_CPU
#define F_CPU 8000000UL // Internal 8MHz clock
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PORTB-pins
  DDRB = 0x10;

  // Loop forever
  while(1) {
    // LED On/Off every other loop
    PORTB ^= 0b00010000;
    _delay_ms(500);
  }
}
