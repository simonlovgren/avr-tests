#ifndef f_CPU
#define F_CPU 9600000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PORTB-pins
  DDRB = 0x10;

  // Loop forever
  while(1) {
    // LED On
    PORTB = 0b00010000;
    _delay_ms(500);
    // LED Off
    PORTB = 0b00000000;
    _delay_ms(500);
  }
}
