/*
 * DoorSensor.c
 *
 *  Created on: Apr 28, 2020
 *      Author: Tiko
 */

#include "../VL53L1X/core/VL53L1X_api.h"
#include "../VL53L1X/core/VL53l1X_calibration.h"
#include "../../Drivers/Libraries/DoorSensor/DoorSensor.h"
//#include "../../Drivers/Libraries/CMD-master/cmd_task.h"
#include "main.h"

// People Counting defines
#define NOBODY 0
#define SOMEONE 1
#define LEFT 0
#define RIGHT 1


//extern CMD_HandleTypeDef uart2CommsHandle;		/* Handle for the control command	*/
/* Exported Variables **********************************************************/
uint32_t PplEntry = 0;
uint32_t PplExit = 0;
int DIST_THRESHOLD_MAX  = 1000;
uint16_t Dist[2] = {0};


/* Private Variables **********************************************************/
uint16_t dev = 0x52;	/* VL53L1X Address	*/

int status = 0;
int8_t error;
uint8_t byteData, sensorState=0;
uint16_t wordData;
uint16_t Distance;
uint8_t RangeStatus;
uint8_t dataReady;
int center[2] = {167,231}; /* these are the spad center of the 2 8*16 zones */
int Zone = 0;


  static int ProcessPeopleCountingData(int16_t Distance, uint8_t zone);


  void DoorSensor_Init(){

		uint32_t tickStart = HAL_GetTick();

	  while (sensorState == 0) {
		  status = VL53L1X_BootState(dev, &sensorState);
		  HAL_Delay(2);

		  if(HAL_GetTick() - tickStart > 1000){
				/* Error -> Reset STM32 */
			  asm("NOP");
			  NVIC_SystemReset();
		  }
	  }

	  /* Initialize and configure the device according to people counting need */
	  status = VL53L1X_SensorInit(dev);
	  status += VL53L1X_SetDistanceMode(dev, 2); /* 1=short, 2=long */
	  status += VL53L1X_SetTimingBudgetInMs(dev, 20); /* in ms possible values [20, 50, 100, 200, 500] */
	  status += VL53L1X_SetInterMeasurementInMs(dev, 20);
	  status += VL53L1X_SetROI(dev, 8, 16); /* minimum ROI 4,4 */
	  if (status != 0) {
		  //printf("Error in Initialization or configuration of the device\n");
	  }

	  status = VL53L1X_StartRanging(dev);   /* This function has to be called to enable the ranging */
  }

void DoorSensor_Tasks(){

	uint32_t tickStart = HAL_GetTick();

	while (dataReady == 0) {
		status = VL53L1X_CheckForDataReady(dev, &dataReady);
		HAL_Delay(2);

		if(HAL_GetTick() - tickStart > 1000){
			DoorSensor_Init();
		}
	}

	dataReady = 0;
	status += VL53L1X_GetRangeStatus(dev, &RangeStatus);
	status += VL53L1X_GetDistance(dev, &Distance);
	status += VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt */
	if (status != 0) {
		//printf("Error in operating the device\n");
	}

	// wait a couple of milliseconds to ensure the setting of the new ROI center for the next ranging is effective
	// otherwise there is a risk that this setting is applied to current ranging (even if timing has expired, the intermeasurement
	// may expire immediately after.
	HAL_Delay(10);
	status = VL53L1X_SetROICenter(dev, center[Zone]);
	if (status != 0) {
		//printf("Error in chaning the center of the ROI\n");
	}

	// inject the new ranged distance in the people counting algorithm
	ProcessPeopleCountingData(Distance, Zone);

	Dist[Zone] = Distance;

	Zone++;
	Zone = Zone%2;


}



static int ProcessPeopleCountingData(int16_t Distance, uint8_t zone) {
    static int PathTrack[] = {0,0,0,0};
    static int PathTrackFillingSize = 1; // init this to 1 as we start from state where nobody is any of the zones
    static int LeftPreviousStatus = NOBODY;
    static int RightPreviousStatus = NOBODY;

    int CurrentZoneStatus = NOBODY;
    int AllZonesCurrentStatus = 0;
    int AnEventHasOccured = 0;

	if (Distance < DIST_THRESHOLD_MAX) {
		// Someone is in !
		CurrentZoneStatus = SOMEONE;
	}

	// left zone
	if (zone == LEFT) {

		if (CurrentZoneStatus != LeftPreviousStatus) {
			// event in left zone has occured
			AnEventHasOccured = 1;

			if (CurrentZoneStatus == SOMEONE) {
				AllZonesCurrentStatus += 1;
			}
			// need to check right zone as well ...
			if (RightPreviousStatus == SOMEONE) {
				// event in left zone has occured
				AllZonesCurrentStatus += 2;
			}
			// remember for next time
			LeftPreviousStatus = CurrentZoneStatus;
		}
	}
	// right zone
	else {

		if (CurrentZoneStatus != RightPreviousStatus) {

			// event in left zone has occured
			AnEventHasOccured = 1;
			if (CurrentZoneStatus == SOMEONE) {
				AllZonesCurrentStatus += 2;
			}
			// need to left right zone as well ...
			if (LeftPreviousStatus == SOMEONE) {
				// event in left zone has occured
				AllZonesCurrentStatus += 1;
			}
			// remember for next time
			RightPreviousStatus = CurrentZoneStatus;
		}
	}

	// if an event has occured
	if (AnEventHasOccured) {
		if (PathTrackFillingSize < 4) {
			PathTrackFillingSize ++;
		}

		// if nobody anywhere lets check if an exit or entry has happened
		if ((LeftPreviousStatus == NOBODY) && (RightPreviousStatus == NOBODY)) {

			// check exit or entry only if PathTrackFillingSize is 4 (for example 0 1 3 2) and last event is 0 (nobobdy anywhere)
			if (PathTrackFillingSize == 4) {
				// check exit or entry. no need to check PathTrack[0] == 0 , it is always the case

				if ((PathTrack[1] == 1)  && (PathTrack[2] == 3) && (PathTrack[3] == 2)) {
					// This an entry
					PplEntry++;


				} else if ((PathTrack[1] == 2)  && (PathTrack[2] == 3) && (PathTrack[3] == 1)) {
					// This an exit
					PplExit++;

				}
			}

			PathTrackFillingSize = 1;
		}
		else {
			// update PathTrack
			// example of PathTrack update
			// 0
			// 0 1
			// 0 1 3
			// 0 1 3 1
			// 0 1 3 3
			// 0 1 3 2 ==> if next is 0 : check if exit
			PathTrack[PathTrackFillingSize-1] = AllZonesCurrentStatus;
		}
	}

	// output debug data to main host machine
	return(0);

}




//	uint8_t string[255] = {0};
//	uint16_t len;
//	extern UART_HandleTypeDef huart2;
//
//	static int prevCount = 0;
//	if (prevCount != PplCounter){
//		prevCount = PplCounter;
//		len = sprintf(string, "%d, %d, %d, %d\n\r", Zone, RangeStatus, Distance, PplCounter);
//		HAL_UART_Transmit(&huart2, string, len, 0xff);
//	}

//	static uint32_t timeStamp;
//
//	if((timeStamp + 500) < HAL_GetTick()){
//		timeStamp = HAL_GetTick();
//		printf("%d, %d, %d, %d\n", Zone, RangeStatus, Distance, PplCounter);
//		len = sprintf(string, "%d, %d, %d, %d\n\r", Zone, RangeStatus, Distance, PplCounter);
//		HAL_UART_Transmit(&huart2, string, len, 0xff);
//	}
