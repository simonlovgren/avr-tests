/*
 Based on CapacitiveSensor module / implementation by Paul Badger.
 See copyright in sense()-function.
*/

/**
 * ----------------------------------------------------------------------------
 * Pre-include defines
 * ----------------------------------------------------------------------------
 */
#ifndef __AVR_ATtiny85__
#define __AVR_ATtiny85__
#endif

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/**
 * ----------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdlib.h>

/**
 * ----------------------------------------------------------------------------
 * Defines
 * ----------------------------------------------------------------------------
 */

/**
 * ***************************************************************************
 * General defines
 * ***************************************************************************
 */

// Pin mode
#define SET_MODE_INPUT( PIN )      ( DDRB &= ~PIN )
#define SET_MODE_OUTPUT( PIN )     ( DDRB  |= PIN )
// Input pin config
#define ENABLE_PULLUP( PIN )       ( PORTB |= PIN )
#define DISABLE_PULLUP( PIN )      ( PORTB &= ~PIN )
// Output pin config
#define SET_HIGH( PIN )            ( PORTB |= PIN )
#define SET_LOW( PIN )             ( PORTB &= ~PIN )
#define SET_TOGGLE( PIN )          ( PORTB ^= PIN )
// Input pin
#define READ_INPUT( PIN )          ( PINB & PIN )

/**
 * ***************************************************************************
 * Timer defines
 * ***************************************************************************
 */


/**
 * ***************************************************************************
 * Application specific defines
 * ***************************************************************************
 */
#define LED1     (uint8_t)( 1 << PB0 )
#define LED2     (uint8_t)( 1 << PB1 )
#define SEND     (uint8_t)( 1 << PB3 ) // Pin to send detect-pule on
#define RECEIVE  (uint8_t)( 1 << PB4 ) // Pin to read cap. touch on

#define CAPSENSE_DETECT_TIMEOUT_MS ( 2000 )
#define CAPSENSE_AUTOCAL_MS        ( 20000 )

/**
 * ----------------------------------------------------------------------------
 * Typedefs
 * ----------------------------------------------------------------------------
 */

/**
 * ----------------------------------------------------------------------------
 * Prototypes
 * ----------------------------------------------------------------------------
 */

/**
 * @brief Initialize board / device.
 */
static void init( void );

/**
 * @brief Configure timer for millis-counter
 */
static void configure_millis_timer ( void );

/**
 * @brief   Check if timestamp has passed.
 * @returns True if passed, False otherwise.
 */
static bool timestamp_passed ( uint32_t timestamp );

/**
 * @brief Get milliseconds since start.
 */
static uint32_t millis ( void );

/**
 * @brief Sense capacitive touch "sensor"
 * @returns >0 if ok, -2 if not
 */
static int sense( void );

/**
 * @brief Sample cap touch sensor
 */
static long raw_sample( uint8_t samples );

/**
 * @brief Sample cap touch sensor, filters base value from result.
 */
static long sample( uint8_t samples );

/**
 * ----------------------------------------------------------------------------
 * Globals
 * ----------------------------------------------------------------------------
 */

/**
 * ----------------------------------------------------------------------------
 * ISR implementations
 * ----------------------------------------------------------------------------
 */
static volatile uint32_t ms_since_start = 0;
ISR (TIMER1_COMPA_vect)
{
    ++ms_since_start;
}

/**
 * ----------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------
 */

/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static void configure_millis_timer ( void )
{
    // Clear timer/counter on compare/match
    TCCR1 |= ( 1 << CTC1 );
    // Divide by 32 -> output compare 250 for 1 ms
    TCCR1 |= ( 1 << CS11 );
    TCCR1 |= ( 1 << CS12 );
    // Output compare register 1A - 250 -> 1 ms
    OCR1A = 250;
    // Enable output compare interrupt for OCR1A
    TIMSK |= ( 1 << OCIE1A );
}

/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static uint32_t millis ( void )
{
    uint32_t milliseconds;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        milliseconds = ms_since_start;
    }
    return milliseconds;
}

/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static bool timestamp_passed ( uint32_t timestamp )
{
    uint32_t current = millis();
    return ( (int32_t)(current-timestamp) > 0 ) ? true : false;
}

/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static void init( void )
{
    // Configure pins (Data Direction Register)
    // PB0 -> output : LED1
    // PB1 -> output : LED2
    // PB3 -> output : Cap. touch charge / pulse
    // PB4 -> input  : Cap. touch read
    SET_MODE_OUTPUT( LED1 );
    SET_MODE_OUTPUT( LED2 );
    SET_MODE_OUTPUT( SEND );
    SET_MODE_INPUT( RECEIVE );

    // Set all outputs to LOW
    SET_LOW( LED1 );
    SET_LOW( LED2 );
    SET_LOW( SEND );

    // Set input to not use pullup
    DISABLE_PULLUP( RECEIVE );

    // Set up millis timer
    configure_millis_timer();
}

/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static int sense( void )
{
    /*
    https://github.com/PaulStoffregen/CapacitiveSensor
    http://www.pjrc.com/teensy/td_libs_CapacitiveSensor.html
    http://playground.arduino.cc/Main/CapacitiveSensor
    Copyright (c) 2009 Paul Bagder
    Updates for other hardare by Paul Stoffregen, 2010-2016

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
    */

    int count = 0;

    // Bleed charge from sense-circuit to get known state

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        SET_LOW( SEND );                // Set send pin to LOW
        SET_MODE_INPUT( RECEIVE );      // Set receive pin to input
        SET_MODE_OUTPUT( RECEIVE );     // Set receive pin to output
        SET_LOW( RECEIVE );             // Set receive pin to LOW
        _delay_us( 10 );
        SET_MODE_INPUT( RECEIVE );      // Set receive pin to input
        SET_HIGH( SEND );               // Set send pin to HIGH
        sei();                          // Re-enable interrupts
    }

    // Check if sensor is connected.
    // Loop while receive pin is LOW and we have not passed our check timeout
    uint32_t timeout = millis() + CAPSENSE_DETECT_TIMEOUT_MS;
    while ( ( false == (bool)READ_INPUT( RECEIVE ) ) && false == timestamp_passed( timeout ) )
    {
        ++count;
    }
    // Sensor seems to not be connected...
    if ( true == timestamp_passed( timeout ) )
    {
        return -2;
    }

    // set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        SET_MODE_OUTPUT( RECEIVE );  // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
        SET_HIGH( RECEIVE );
        SET_MODE_INPUT( RECEIVE );    // receivePin to INPUT (pullup is off)
        DISABLE_PULLUP( RECEIVE );
        SET_LOW(SEND);                // sendPin LOW
    }

    // Read sensor pin
    // Loop while receive pin is HIGH and we have not passed our check timeout
    timeout = millis() + CAPSENSE_DETECT_TIMEOUT_MS;
    while ( ( true == (bool)READ_INPUT( RECEIVE ) ) && false == timestamp_passed( timeout ) )
    {
        ++count;
    }
    // Sensor seems to not be connected...
    if ( true == timestamp_passed( timeout ) )
    {
        return -2;
    } else {
        return count;
    }
}


/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static long raw_sample( uint8_t samples )
{
    long result = 0;
    for ( uint8_t i = 0; i < samples; ++i )
    {
        int senseval = sense();
        if ( senseval < 0 )
        {
            return senseval;
        }
        result += senseval;
    }
    return result;
}

/**
 * ***************************************************************************
 * Function
 * ***************************************************************************
 */
static long sample( uint8_t samples )
{
    /*
    https://github.com/PaulStoffregen/CapacitiveSensor
    http://www.pjrc.com/teensy/td_libs_CapacitiveSensor.html
    http://playground.arduino.cc/Main/CapacitiveSensor
    Copyright (c) 2009 Paul Bagder
    Updates for other hardare by Paul Stoffregen, 2010-2016

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
    */

    long result = 0;
    static uint32_t lastCal = 0;
    static long     leastTotal = 0x0FFFFFFFL;
    if (samples == 0) return 0;

    for (uint8_t i = 0; i < samples; i++) {    // loop for samples parameter - simple lowpass filter
        int senseVal = sense();
        if (senseVal < 0)  return -2;   // variable over timeout
        result += senseVal;
    }

    if ( (millis() - lastCal > CAPSENSE_AUTOCAL_MS ) && abs(result  - leastTotal) < (int)(.10 * (float)leastTotal) ) {
        leastTotal = 0x0FFFFFFFL;          // reset for "autocalibrate"
        lastCal = millis();
    }

    // routine to subtract baseline (non-sensed capacitance) from sensor return
    if (result < leastTotal)
    {
        leastTotal = result; // set floor value to subtract from sensed value
    }
    return (result - leastTotal);
}

/**
 * ----------------------------------------------------------------------------
 * Entrypoint
 * ----------------------------------------------------------------------------
 */

int main(void) {
    // Initialize board
    init();

    while( 1 )
    {
        long senseVal = sample( 50 );
        if ( senseVal < 0 )
        {
            // stop blinking if bad val return
            SET_LOW( LED2 );
            SET_HIGH( LED1 );
        }
        else
        {
            SET_LOW( LED1 );
            // Touch input reaction
            if  ( senseVal > 300 )
            {
                SET_HIGH( LED2 );
            }
            else
            {
                SET_LOW( LED2 );
            }

        }

    }

}
