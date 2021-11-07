#include <msp430.h> 
#include "msp430BuiltInIO.h"
#include "global.h"
#include "boardConfig.h"
#include "echoStateMachine.h"


/**
 * main.c
 */

volatile int timerFlag = 0;
volatile int timerCounter = 0;

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

    //echoState = waitForPulse;
    echoState = beforeFirstPulse;

    while(1) {

        // read echo input
        int echo = P2IN & (0x01 << 5);
        echoTick(echo);

        // making sure ISR is working - red LED should blink
        if (timerCounter >= 10) {
            toggleLED1();
            timerCounter = 0;
        }

        timerFlag = 0;
    }

    return 0;
}

// Timer0_A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer0Interrupt (void)
{
    // do nothing for now

}

// Timer1_A0 interrupt service routine
#pragma vector = TIMER1_A0_VECTOR
__interrupt void timer1Interrupt (void)
{
    timerCounter++;
    timerFlag = 1;
}
