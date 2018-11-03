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





// #include <avr/io.h>

// int main(void) {
//   // Configure PB0 as output
//   DDRB = 0x1;

//   // Set up timer for blinking
//   TCCR0A = 1 << COM0A0 | 0 << WGM00;  // Toggle OC0A, CTC mode.
//   TCCR0B = 1 << WGM02 | 3 << CS00;    // CTC mode; use OCR0A; /64
//   OCR0A = 15624;                     // 1 second; ie 0.5Hz

//   // Loop forever
//   while(1);
// }
