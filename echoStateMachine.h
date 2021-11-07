/*
 * echoStateMachine.h
 *
 *  Created on: Nov 4, 2021
 *      Author: alex
 */

#ifndef ECHOSTATEMACHINE_H_
#define ECHOSTATEMACHINE_H_

enum echoStates {beforeFirstPulse, risingEdge, duringPulse, afterPulse} echoState;
void echoTick(int echoInput);
void echoStateTransition(int echoInput);
void echoStateActions();


#endif /* ECHOSTATEMACHINE_H_ */
