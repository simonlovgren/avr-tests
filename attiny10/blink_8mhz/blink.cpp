#ifndef F_CPU
#define F_CPU 8000000UL // Internal clock at 8MHz
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Set internal clock to 8MHz
  CCP    = 0xD8; // CCP register must contain correct signature to change clock (0xD8)
  CLKPSR = 0x0;  // Set Clock Prescaler Register to 0 (do not divide internal 8MHz clock)

  // Configure PORTB-pins
  DDRB = 0x01;

  // Loop forever
  while( 1 ) {
    // LED On/Off every other loop
    PORTB ^= 0x01;
    _delay_ms(500);
  }
}
