/*
This file is meant to be included at the top of this projects main.c file as well as other files that may need it.
Below are includes from ev3dev-c library and necessary definitions to be used troughout the program.
This file also includes declarations och global variables.
The purpose of this is to give the files in need of these definitions and declarations a cleaner appearance
*/

#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) // Defines Sleep() to pause execution of the program for given amount of time in milliseconds.

// Defines the physical ports on the EV3 "brick" to more descriptive names.
#define MOTOR_LEFT    	OUTA
#define MOTOR_CENTER   	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_RIGHT    	OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_2		IN2
#define SENSOR_3		IN3
#define SENSOR_4		IN4

#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) // Definition used to refer to both ports for the EV3:s wheel motors at the same time.

// Global variables
int maxSpeedCenter;
int maxSpeedWheels;