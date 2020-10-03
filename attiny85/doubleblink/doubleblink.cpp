#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PORTB-pins
  DDRB = 0x01;

  // Loop forever
  while(1) {
    // LED On/Off every other loop
    PORTB |= 0b00000001;
    PORTB &= 0b11111101;
    _delay_ms(250);
    PORTB |= 0b00000010;
    PORTB &= 0b11111110;
    _delay_ms(250);
  }
}
