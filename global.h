
#include "msp430BuiltInIO.h"

/*
 * global.h
 *
 *  Created on: Oct 23, 2021
 *      Author: alex
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define SMCLOCK_HZ 1000000L  // Hz
#define INTERRUPT_TIMER_PERIOD 50000L // 50,000us, 50ms
#define TRIGGER_PULSE 100L // 100us
#define TRIGGER_PERIOD 50000L // 50,000us, 50ms
typedef enum {false, true} bool;

#endif /* GLOBAL_H_ */
