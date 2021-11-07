#include "echoStateMachine.h"
#include "global.h"

unsigned long echoCounter = 0;
unsigned long periodCounter = 0;
double echoDutyCycle;

void echoTick(int echoInput) {
    echoStateTransition(echoInput);
    echoStateActions();
}

void echoStateTransition(int echoInput) {
    switch (echoState) {
    case beforeFirstPulse:
        if (echoInput != 0) {
            echoState = duringPulse;
        } else {
            echoState = beforeFirstPulse;
        }
        break;
    case risingEdge:
        echoState = duringPulse;
        break;
    case duringPulse:
        if (echoInput != 0) {
            echoState = duringPulse;
        } else {
            echoState = afterPulse;
        }
        break;
    case afterPulse:
        if (echoInput != 0) {
            echoState = risingEdge;
        } else {
            echoState = afterPulse;
        }
        break;
    default:
        echoState = beforeFirstPulse;
        break;
    }
}

void echoStateActions() {
    switch (echoState) {
        case beforeFirstPulse:
            echoCounter = 0;
            periodCounter = 0;
            break;
        case risingEdge:
            // do calculations and reset counters
            echoDutyCycle = (double)echoCounter / (double)periodCounter;
            double echoLength = echoDutyCycle * TRIGGER_PERIOD;
            double echoLengthMS = echoLength / 1000;

            // reset counters for next pulse
            echoCounter = 0;
            periodCounter = 0;

            break;
        case duringPulse:
            echoCounter++;
            periodCounter++;
            break;
        case afterPulse:
            periodCounter++;
            break;
        default:
            echoCounter = 0;
            periodCounter = 0;
            break;
        }
}

