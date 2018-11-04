// Running at default frequency (1MHz)

#include <avr/io.h>

int main(void) {
  // Configure PORTB-pins
  DDRB = 0x01;

  // Set up timer for blinking
  TCCR0A = 1 << COM0A0 | 0 << WGM00;  // Toggle OC0A, CTC mode. Toggle 0C0A on compare match.
  TCCR0B = 1 << WGM02 | 3 << CS00;    // CTC mode; use OCR0A; /64 (prescaler divide by 64)
  OCR0A = 15624;                     // 1 second; ie 0.5Hz

  // Loop forever and do whatever. The peripheral takes care of all...
  while( 1 ) {}
}
