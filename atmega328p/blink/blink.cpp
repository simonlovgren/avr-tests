#ifndef F_CPU
#define F_CPU 8000000UL // Internal 8MHz clock
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Configure PD2 as output
  DDRD = 0x04;

  // Loop forever
  while(1) {
    // LED On/Off every other loop
    PORTD ^= 0b00000100;
    _delay_ms(500);
  }
}
