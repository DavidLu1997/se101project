//Track.h
//Used for tracking-related functions

#ifndef TRACK_H
#define TRACK_H

extern "C" {
#include <delay.h>
#include <FillPat.h>
#include <I2CEEPROM.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
}

#include "watch.h"
#include "misc.h"
#include "socket.h"

//Function calling delays, ms
#define DRAW_DELAY 50
#define STEP_DELAY 100
#define ACCEL_DELAY 1
#define TEMP_DELAY 1

//Past range to check steps, ms
#define STEP_RANGE 500

//Past range to check temperature
#define TEMP_RANGE 500

//Past Range to store heartbeat data
#define HEART_RANGE 60000 //1 minute

//Acceleration data memory locations
#define	X_ADDR 0x32
#define	Y_ADDR 0x34
#define	Z_ADDR 0x36

//Step Tracking Params
#define STEP_SENSITIVITY 1.0 //Higher the number = less sensitive
#define STEP_DISTANCE 0.7874 //Distance travelled per step in m
#define WEIGHT 70 //Weight of user in kg

//Number of steps since start
int steps = 0;

//Number of heartbeats since start
int beats = 0;

//Heartbeat per second
int heartBeats[HEART_RANGE / 1000];

//Accelerometer data for past STEP_RANGE
double data[STEP_RANGE];

//Temperature data for past TEMP_RANGE
double temp[TEMP_RANGE];

//Initialization function
//Only called once
void initTrack() {
	int i;
	for(i = 0 ; i < HEART_RANGE / 1000; i++)
		heartBeats[i] = 0;
	for(i = 0; i < STEP_RANGE; i++)
		data[i] = 0;
	for(i = 0; i < TEMP_RANGE; i++)
		temp[i] = 0;
	return;
}


//Receives input
void trackInput(int input, int selected) {
	switch(input) {
		case SWITCH_A:
			//TODO: Switch A
			break;
		case SWITCH_B:
			//TODO: Switch B
			break;
		case BUTTON_A:
			//TODO: Button A
			break;
		case BUTTON_B:
			//TODO: Button B
			break;
		case POT:
			//TODO: Smoke Weed
			break;
		default:
			//SUM TING WONG
			//REALLY WONG
			//REALLY REALLY WONG
			//REALLY REALLY REALLY WONG
			//RILEY WONG
			break;
	}
}

char I2CGenTransmit(char * pbData, int cSize, bool fRW, char bAddr);
bool I2CGenIsNotIdle();

//Inputs the acceleration data into the given int[] in the form of
//[0] - X acceleration
//[1] - Y
//[2] - Z
//ACCEL_DELAY
void getAccelerationData(){
	//Probably can be done in a loop, but this is probably more readable

	int d[3];

	char rgchReadAcclX[3] = {0};
	char rgchReadAcclY[3] = {0};
	char rgchReadAcclZ[3] = {0};

	rgchReadAcclX[0] = X_ADDR;
	rgchReadAcclY[0] = Y_ADDR;
	rgchReadAcclZ[0] = Z_ADDR;

	I2CGenTransmit(rgchReadAcclX, 2, READ, ACCLADDR);
	I2CGenTransmit(rgchReadAcclY, 2, READ, ACCLADDR);
	I2CGenTransmit(rgchReadAcclZ, 2, READ, ACCLADDR);

	d[0] = (rgchReadAcclX[2] << 8) | rgchReadAcclX[1];
	d[1] = (rgchReadAcclY[2] << 8) | rgchReadAcclY[1];
	d[2] = (rgchReadAcclZ[2] << 8) | rgchReadAcclZ[1];

	//Store magnitude of acceleration in data
	data[millis() % STEP_RANGE] = sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
}

//Draws current function, continuously called
//DRAW_DELAY
void drawTrack() {
	return;
}

//Check step, continuously called, returns 1 if step occurred in past STEP_RANGE ms
//STEP_DELAY
int checkStep() {
	getAccelerationData();
	//Calculate average acceleration over STEP_RANGE
	int i;
	double avg = 0;
	for(i = 0; i < STEP_RANGE; i++) {
		avg += data[i];
	}
	avg = avg / STEP_RANGE;

	//Greater than sensitivity
	if(avg >= STEP_SENSITIVITY) {
		return 1;
	}

	return 0;
}

//Check temperature, continuously called, returns 1 if heartbeat occurred in past TEMP_RANGE ms
//TEMP_DELAY
int checkHeart() {
	heartBeats[millis() / 1000 % HEART_RANGE]++;
	return 0;
}

//Get temperature data, continuously called
//TEMP_DELAY
void getTemperature() {
	//TODO: Get thermometer reading
	data[millis() % TEMP_RANGE] = 0.0;
}

//Get current steps
int getSteps() {
	return steps;
}

//Get current distance travelled in m
double getDistance() {
	return steps * STEP_DISTANCE;
}

//Get calories burnt
int getCalories() {
	double kmh = (getDistance() / 1000.0) / (millis() / 1000.0) * 3600;
	//Formula obtained from ShapeSense.com
	return (0.0215 * pow(kmh, 3) - 0.1765 * pow(kmh, 2) + 0.8710 * kmh + 1.8600) * WEIGHT * (millis() / 1000 * 3600);
}

//Reset steps
void resetSteps() {
	steps = 0;
}

//Set steps
void setSteps(int s) {
	steps = s;
}

//TODO: Put the in the setup code
void initAccelerometer(){
	GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);
}

char I2CGenTransmit(char * pbData, int cSize, bool fRW, char bAddr) {

  int 		i;
  char * 		pbTemp;

  pbTemp = pbData;

  /*Start*/

  /*Send Address High Byte*/
  /* Send Write Block Cmd*/
  I2CMasterSlaveAddrSet(I2C0_BASE, bAddr, WRITE);
  I2CMasterDataPut(I2C0_BASE, *pbTemp);

  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

  DelayMs(1);

  /* Idle wait*/
  while(I2CGenIsNotIdle());

  /* Increment data pointer*/
  pbTemp++;

  /*Execute Read or Write*/

  if(fRW == READ) {

    /* Resend Start condition
	** Then send new control byte
	** then begin reading
	*/
    I2CMasterSlaveAddrSet(I2C0_BASE, bAddr, READ);

    while(I2CMasterBusy(I2C0_BASE));

    /* Begin Reading*/
    for(i = 0; i < cSize; i++) {

      if(cSize == i + 1 && cSize == 1) {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

        DelayMs(1);

        while(I2CMasterBusy(I2C0_BASE));
      }
      else if(cSize == i + 1 && cSize > 1) {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

        DelayMs(1);

        while(I2CMasterBusy(I2C0_BASE));
      }
      else if(i == 0) {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

        DelayMs(1);

        while(I2CMasterBusy(I2C0_BASE));

        /* Idle wait*/
        while(I2CGenIsNotIdle());
      }
      else {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

        DelayMs(1);

        while(I2CMasterBusy(I2C0_BASE));

        /* Idle wait */
        while(I2CGenIsNotIdle());
      }

      while(I2CMasterBusy(I2C0_BASE));

      /* Read Data */
      *pbTemp = (char)I2CMasterDataGet(I2C0_BASE);

      pbTemp++;

    }

  }
  else if(fRW == WRITE) {

    /*Loop data bytes */
    for(i = 0; i < cSize; i++) {
      /* Send Data */
      I2CMasterDataPut(I2C0_BASE, *pbTemp);

      while(I2CMasterBusy(I2C0_BASE));

      if(i == cSize - 1) {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

        DelayMs(1);

        while(I2CMasterBusy(I2C0_BASE));
      }
      else {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

        DelayMs(1);

        while(I2CMasterBusy(I2C0_BASE));

        /* Idle wait */
        while(I2CGenIsNotIdle());
      }

      pbTemp++;
    }

  }

  /*Stop*/

  return 0x00;

}

bool I2CGenIsNotIdle() {

  return !I2CMasterBusBusy(I2C0_BASE);

}

#endif // TRACK_H
