#ifndef f_CPU
#define F_CPU 1200000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRB = 0x10;
  while(1) {
    PORTB = 0b00010000;
    _delay_ms(500);
    PORTB = 0b00000000;
    _delay_ms(500);
  }
}
