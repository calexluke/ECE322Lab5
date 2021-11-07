#include <msp430.h>
#include "boardConfig.h"
#include "global.h"

/*
 * boardConfig.c
 *
 *  Created on: Nov 6, 2021
 *      Author: alex
 */


void configTimersForTrigger() {
    // Configure Timer0_A
    // PWM Period
    TA0CCR0 = TRIGGER_PERIOD;
    // CCR1 reset/set
    TA0CCTL1 = 0x00E0;
    // TACCR0 interrupt enabled
    TA0CCTL0 = CCIE;

    // Configure TimerA0 mode and GO
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;   // SMCLK, up mode, clear TAR

    TA0CCR1 = TRIGGER_PULSE;

}

void configInterruptTimer() {
    // Configure Timer1_A period
    TA1CCR0 = INTERRUPT_TIMER_PERIOD;

    // interrupt enabled
    TA1CCTL0 = CCIE;

    // Configure TimerA1 mode and GO
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;   // SMCLK, up mode, clear TAR
}

void configIOForTrigger() {
    // setup P1.6 with TA0CCR1 output signal
    // ft6989 datasheet section 6.11.24.4, Table 6-21
    // registers P1DIR, P1SEL1, P1SEL0
    // datasheet p.97 (NOT FAMILY USER GUIDE)
    P1DIR |= (0x01 << 6);
    P1SEL1 |= (0x01 << 6);
    P1SEL0 |= (0x01 << 6);
}

void configIOForEcho() {
    // P2.5 NO pullup or pulldown resistor
    // echo input
    P2DIR &= ~(0x01 << 5);
    P2REN &= ~(0x01 << 5);
    P2OUT &= ~(0x01 << 5);

    // pullup/pulldown resistors for I/O config:
    // family user guide table 12-1 p. 368
    // set bit to 0 with
    // register &= ~(0x01 << n)
}
