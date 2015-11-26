#ifndef SOCKET_H
#define SOCKET_H

#include "track.h"
#include "watch.h"
#include "misc.h"

//Menu delay
#define MENU_DELAY 500

//Variables for displaying steps
int step;
char stringSteps[5];

//Button variables
long btn1;
long btn2;

//Switch variables
long swt1;

//Int variables for time
long hours = 19;
long minutes = 0;
long seconds = 0;

//Char variables for time
char stringSeconds[2];
char stringMinutes[2];
char stringHours[2];

//Integer ID for potentiometer
#define POT 420

//Potentiometer
int pot = 0;

//Menu definitions
#define MAIN 0
#define TRACK 1
#define WATCH 2
#define TIMER 3
#define ALARM 4
#define STOPWATCH 5
#define SETTINGS 6
#define SETTIME 7
#define SETSTEPS 8
#define SETHEARTBEATS 9

//Active menu
int activeMenu = 0;

//Declarations
void initSocket();
void drawMenu();
void drawActiveMenu();

//Draws currently active menu
void drawActiveMenu() {
	if(millis() % MENU_DELAY) {
		switch(activeMenu) {
			case MAIN:
				drawMenu();
				break;
			case TRACK:
				drawTrack();
				break;
			case WATCH:
				drawWatch();
				break;
			case TIMER:
				drawTimer();
				break;
			case ALARM:
				drawAlarm();
				break;
			case STOPWATCH:
				drawStopWatch();
				break;
			case SETTINGS:
				drawSettings();
				break;
			case SETTIME:
				drawSetTimes();
				break;
			case SETSTEPS:
				drawSetSteps();
				break;
			case SETHEARTBEATS:
				drawSetHeartbeats();
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
}

//Initializes socket functions
void initSocket() {
	OrbitOledInit();
	OrbitOledClear();
}

//Draws the current menu item
void drawMenu() {

	//Draws menu every MENU_DELAY
        OrbitOledClear();

        step = getSteps();

        //Displaying steps text
        OrbitOledSetCursor(4 , 1);
        OrbitOledPutString("Steps: ");

        //Displaying steps number
        itoa(step, stringSteps, 10);
        OrbitOledSetCursor(11, 1);
        OrbitOledPutString(stringSteps);

        hours = getDate().hour;
        minutes = getDate().minute;
        seconds = getDate().second;

        //Convert ints to strings, then prints them to the screen
        itoa(hours, stringHours, 10); 
        OrbitOledSetCursor(3, 3);
        OrbitOledPutString(stringHours);
        
        OrbitOledSetCursor(5, 3);
        OrbitOledPutString(":");

        //Convert ints to strings, then prints them to the screen
        itoa(hours, stringHours, 10); 
        OrbitOledSetCursor(3, 2);
        OrbitOledPutString(stringHours);
        

        OrbitOledSetCursor(5, 2);
        OrbitOledPutString(":");
        
        //Prints minutes
        itoa(minutes, stringMinutes, 10);
        OrbitOledSetCursor(7, 3);
        OrbitOledPutString(stringMinutes);

        OrbitOledSetCursor(9, 3);
        OrbitOledPutString(":");
        
        //Printes seconds
        itoa(seconds, stringSeconds, 10);
        OrbitOledSetCursor(11, 3);
        OrbitOledPutString(stringSeconds);
        OrbitOledUpdate;

    //Bottom button
    GPIOPinTypeGPIOInput(BTN1Port, BTN1);
    btn1 = GPIOPinRead(BTN1Port, BTN1);
        
    //Top Button
    GPIOPinTypeGPIOInput(BTN2Port, BTN2);
    btn2 = GPIOPinRead(BTN2Port, BTN2);

    GPIOPinTypeGPIOInput(SWTPort, SWT1 | SWT2);
    swt1 = GPIOPinRead(SWT1Port, SWT1);

    //Goes to track screen
    if (btn1 == BTN1) {
        activeMenu = TRACK;
    }
    //Goes to time screen
    if (btn2 == BTN2) {
        activeMenu = TIME;
    }
    //Turn light on when switch is flipped, should actually go to settings during implmentation
    if (swt1 == SWT1) {
        activeMenu = SETTINGS;
    }
    else {
        activeMenu = MAIN;
    }
}


#endif //SOCKET_H

