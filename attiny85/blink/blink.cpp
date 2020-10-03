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
    // LED On
    PORTB ^= 0b00000001;
    _delay_ms(500);
  }
}
