/**
 * MIT License
 *
 * Copyright (c) 2017 Simon Lövgren
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * Based on http://kineticsandelectronics.com/sleep.html
 *
 * This program is written to use an external 32.768kHz external crystal for
 * the internal timer with the ATMega running on the internal 8MHz crystal
 * oscillator, in order to function as a simple RTC and drive an industrial
 * (centrally controlled) clock.
 *
 * The industrial clock (LM Ericsson) works by recieving a 24V pulse every
 * minute from a central master clock. The polarity of the pulse is switched
 * other every minute to drive the clock forward, as the clock mechanism
 * consists of a coil that turns the mechanism. If the polarity is not switched,
 * at most the clock will move one minute.
 *
 * This solution uses a single H-bridge (DRV8871) to alternate polarity when
 * sending the signal and for this two outputs (PD1 and PD2) are used:
 *           ____
 *          |    |
 * PD2 ------     ----------/ /---------------------------- ...
 *                                            ____
 *                                           |    |
 * PD1 ----------------------------/ /--------     -------- ...
 *
 */

/**
 * Define the use of the 8MHz (here internal) clock frequency if
 * not already defined somewhere else.
 */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef __AVR_ATmega8__
#define __AVR_ATmega8__
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/**
 * ----------------------------------------------------------------------------
 * Defines
 * ----------------------------------------------------------------------------
 */

/**
 * Time period of HIGH pulse to clock mechanism.
 */
#define MOTOR_PULSE_DURATION_MS  100


/**
 * ----------------------------------------------------------------------------
 * Type definitions
 * ----------------------------------------------------------------------------
 */

/**
 * Sets up TIMER2 and the one-second overflow interrupt we'll use.
 */
void rtc_init( void );

/**
 * Puts the CPU to sleep until next interrupt from TIMER2.
 */
void gnight( void );

/**
 * The main application.
 */
void loop( void );


/**
 * ----------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------
 */

/**
 * Counter to keep track of what second we are on.
 */
static volatile unsigned long seconds = 0;

/**
 * Indication of which polarity we are to pulse with.
 */
static uint8_t direction = 0;

/**
 * ----------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------
 */

/**
 * Entry point of our application.
 */
int main()
{
    /** Configure PD0-2 as outputs. */
    DDRD = 0x07;

    /** Initialize timer. */
    rtc_init();

    /** Enable interrupts. */
    sei();

    /** Set sleep mode. */
    set_sleep_mode( SLEEP_MODE_PWR_SAVE );

    /** Start application */
    while( 1 )
    {
        loop();
    }
}


/**
 * Our main application that will loop forever.
 */
void loop()
{
    /**
     * Toggle pin PD2 (LED) on/off every second.
     */
    PORTD ^= 0b00000100;

    /**
     * If one minute passed, pulse MOTOR_PULSE_DURATION_MS signal. We must
     * also alternate polarity of the pulse sent to the clock, so every
     * other time we pulse the "other way" of the H-bridge.
     *
     * 0bxxxxxx10 -> 0bxxxxxx01 -> 0bxxxxxx10 -> ...(contd.)
     */
    if ( seconds == 0 )
    {
        if ( direction == 0 ){
            PORTD |= 0x02;
            _delay_ms( MOTOR_PULSE_DURATION_MS );
            PORTD &= 0xFD;
            /** Switch polarity for next pulse. */
            direction = 1;
        } else {
            PORTD |= 0x01;
            _delay_ms( MOTOR_PULSE_DURATION_MS );
            PORTD &= 0xFE;
            /** Switch polarity for next pulse. */
            direction = 0;
        }
    }

    /** Put CPU to sleep until next interrupt. */
    gnight();
}


/**
 * Timer setup.
 */
void rtc_init(void)
{
    TCCR2A = 0x00;  /** Overflow */
    TCCR2B = 0x05;  /** Prescaler 5 gives 1s */
    TIMSK2 = 0x01;  /** Enable TIMER2A overflow interrupt. */
    ASSR   = 0x20;  /** Enable asynchronous mode. */
}


/**
 * CPU Sleep.
 */
void gnight()
{
    /** Enable CPU sleep. */
    sleep_enable();

    /**
     * There is a bug with the TIMER2 overflow interrupt in asynchronous mode
     * and going to sleep that causes the interrupt to fire twice, thus making
     * stuff not work properly. The solution is to wait a clock cycle or two until
     * the timer counter is no longer 0 (here we wait at least 2 cycles).
     */
    if ( TCNT2 == (uint8_t)0xff || TCNT2 == 0 || TCNT2 == 1 )
    {
        while (TCNT2 < 2);
    }
    /** Go to sleep. */
    sleep_mode();
    /** Disable sleep on wake-up until this function is called again. */
    sleep_disable();
}


/**
 * ----------------------------------------------------------------------------
 * Interrupt handlers
 * ----------------------------------------------------------------------------
*/

/**
 * Interrupt handler for TIMER2 Overflow interrupt.
 */
ISR( TIMER2_OVF_vect )
{
    /** Increment seconds and reset to 0 if 60 is reached. */
    if( ++seconds == 60 ){
      seconds = 0;
    }
}
