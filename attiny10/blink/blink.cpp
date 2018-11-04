#ifndef F_CPU
#define F_CPU 1000000UL // Internal 1MHz clock
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PORTB-pins
  DDRB = 0x01;

  // Loop forever
  while( 1 ) {
    // LED On/Off every other loop
    PORTB ^= 0x01;
    _delay_ms(500);
  }
}
