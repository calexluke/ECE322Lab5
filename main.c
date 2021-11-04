#include <msp430.h> 
#include "msp430BuiltInIO.h"
#include "global.h"
//#include "echoStateMachine.h"


/**
 * main.c
 */

#define INTERRUPT_TIMER_PERIOD_HZ (1000*10L)  // 100 us or 10khz
#define INTERRUPT_TIMER_PERIOD (SMCLOCK_HZ / INTERRUPT_TIMER_PERIOD_HZ)

#define TRIGGER_PERIOD_HZ 10L  // 10Hz
#define TRIGGER_PULSE_US 100L
#define TRIGGER_PULSE (TRIGGER_PULSE_US * (SMCLOCK_HZ / 1000000L))

// convert to timer clock cycles
#define TRIGGER_PERIOD (SMCLOCK_HZ / TRIGGER_PERIOD_HZ)

int timerFlag = 0;
int timerCounter = 0;


void configInterruptTimer();
void configTimersForTrigger();
void configIOForTrigger();
void configIOForEcho();

enum echoStates {waitForPulse, countingTime, calculateLength} echoState;
void echoTick(int echoInput);
void echoStateTransition(int echoInput);
void echoStateActions();

int counter = 0;
int timerPeriodUS = 100;
int pulseTimeUS = 0;



int main(void)
{

    // stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    configureBuiltInIO();
    configInterruptTimer();
    configTimersForTrigger();
    configIOForTrigger();
    configIOForEcho();

    PM5CTL0 &= ~LOCKLPM5;

    // enable interrupts
    __bis_SR_register(GIE);

    echoState = waitForPulse;

    while(1) {

        // read echo input
        int echo = P2IN & (0x01 << 5);
        // setLED2 based on echo
        if (echo != 0) {
            setLED2(ON);
        } else {
            setLED2(OFF);
        }


        if (timerFlag == 1) {
            echoTick(echo);
        }

        timerFlag = 0;
    }

    return 0;
}

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
    // P2.5 NO pullup resistor
    // echo input
    P2DIR &= ~(0x01 << 5);
    P2REN &= ~(0x01 << 5);
    P2OUT &= ~(0x01 << 5);

    // pullup/pulldown resistors for I/O config:
    // family user guide table 12-1 p. 368
    // set bit to 0 with
    // register &= ~(0x01 << n)
}

void echoTick(int echoInput) {
    echoStateTransition(echoInput);
    echoStateActions();
}

// waitForPulse, countingTime, calculateLength
void echoStateTransition(int echoInput) {
    switch (echoState) {
    case waitForPulse:
        if (echoInput != 0) {
            echoState = countingTime;
        } else {
            echoState = waitForPulse;
        }
        break;
    case countingTime:
        if (echoInput != 0) {
            echoState = countingTime;
        } else {
            echoState = calculateLength;
        }
        break;
    case calculateLength:
        echoState = waitForPulse;
        break;
    default:
        echoState = waitForPulse;
        break;
    }
}

void echoStateActions() {
    switch (echoState) {
        case waitForPulse:
            counter = 0;
            break;
        case countingTime:
            counter++;
            break;
        case calculateLength:
            pulseTimeUS = counter * timerPeriodUS;


            break;
        default:
            counter = 0;
            break;
        }
}

// Timer1_A0 interrupt service routine
#pragma vector = TIMER1_A0_VECTOR
__interrupt void timer1Interrupt (void)
{
    timerFlag = 1;

}
