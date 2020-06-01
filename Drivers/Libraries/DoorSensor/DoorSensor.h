/*
 * DoorSensor.h
 *
 *  Created on: Apr 28, 2020
 *      Author: Tiko
 */

#ifndef DOORSENSOR_H_
#define DOORSENSOR_H_


	#define PplCounter 		(PplEntry - PplExit)
	extern uint32_t PplEntry;
	extern uint32_t PplExit;
	extern int DIST_THRESHOLD_MAX;
	extern uint16_t Dist[2];


	void DoorSensor_Init();
	void DoorSensor_Tasks();

#endif /* DOORSENSOR_H_ */
