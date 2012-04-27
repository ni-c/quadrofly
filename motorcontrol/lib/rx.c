/**
 * Quadrofly Software (http://quadrofly.ni-c.de)
 *  
 * @file 	motorcontrol/lib/rx.c
 * @brief 	Unit to read the signal from the RX receiver
 * @author 	Willi Thiel (wthiel@quadrofly.ni-c.de)
 * @date 	Apr 27, 2012
 */

#include "main.h"
#include "global_def.h"

#ifdef RX_AVAILABLE
#include "i2cslave.h"
#include "motorcontrol.h"

// Includes
#include <avr/io.h>
#include <avr/interrupt.h>

// Global Vars
volatile uint16_t pulse = 0; /*!< Number of timer pulses */
volatile uint8_t channelIndex = 0; /*!< Current channel index */
volatile uint8_t channelPos[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; /*!< Positions of the channel */

ISR( TIMER1_COMPA_vect )
{
    /* 1 tick = 200µs */
    TCNT1 = 0;
    pulse++;
}

ISR( PCINT0_vect )
{
    /* Read pulseTime in µs */
    uint16_t pulseTime = pulse * 200 + 4 * TCNT1 / 5;

    /* Reset Timer */
    TCNT1 = 0;
    pulse = 0;

    /* pulse > 3000 µs => Sync */
    if (pulseTime > 3000)
    {
        /* Write channelvalues to I2C buffer */
        for (int i = 0; i < 8; ++i) {
            i2c_buffer[MC_RC_CHANNEL_1+i] = channelPos[i];
        }
        /* Start from the beginning */
        channelIndex = 0;
    }
    else if (channelIndex < 8)
    {
        /* Save channel position and increase servo index */
        channelPos[channelIndex++] = pulseTime / 200;
    }
}
#endif /* RX_AVAILABLE */

/**
 * Initialization of the RX part
 */
void rx_init(void)
{
#ifdef RX_AVAILABLE
    DDRB &= ~(1 << PB1);
    PORTB |= (1 << PB1);

    /* Timer 1 */
    TCCR1A |= ((1 << CS10) | (1 << CS12)); /* Prescaler 16 - 800ns */
    TIMSK1 |= (1 << OCIE1A); /* Timer/Counter1, Output Compare A Match Interrupt Enable */
    OCR1A = 250; /* 200µs */

    /* Enable Pin Change Interrupt on falling edge */
    MCUCR |= (1 << ISC01);
    PCMSK1 = 0x00;

    /* Enable PCINT on PB1 */
    PCMSK1 |= (1 << PCINT1);
#endif /* RX_AVAILABLE */
}
