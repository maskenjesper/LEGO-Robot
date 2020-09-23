///////////////////// Includes ///////////////////////////
#include <stdio.h>
#include "brick.h"
#include <unistd.h>
//////////////////////////////////////////////////////////

////////////////// General definitions ///////////////////
#define Sleep( msec ) usleep(( msec ) * 1000 ) // Defines Sleep() to pause execution of the program for given amount of time in milliseconds.
//////////////////////////////////////////////////////////

////////////////// Physical definitions //////////////////
#define MOTOR_LEFT    	OUTA
#define MOTOR_CENTER   	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_RIGHT    	OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_2		IN2
#define SENSOR_3		IN3
#define SENSOR_4		IN4
#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) // Definition used to refer to both ports for the EV3:s wheel motors at the same time.
///////////////////////////////////////////////////////////

///////////////// Global variables ////////////////////////
int maxSpeedCenter;
int maxSpeedWheels;
///////////////////////////////////////////////////////////

/////////////// Function Declarations /////////////////////
// Initialization
void initEverything();
void unInitEverything();
void motorsInit();
// Movement
void moveForward(int distance); // Distance in cm
void moveBackward(int distance); // Distance in cm
void rotate(int degrees);
// Actions
void releaseBook();
///////////////////////////////////////////////////////////

////////////////////// Main ///////////////////////////////
int main()
{
    initEverything();

    moveForward(5000);
    moveBackward(5000);
    rotate(5000);

    unInitEverything();

    return 0;
}
//////////////////////////////////////////////////////////

////////////////// Movement //////////////////////////////
void moveForward(int distance)
{
    // Run motor 1 and 2 forwards at the same speed
    // Keep running until the "distance" has been met

    tacho_set_speed_sp(MOTOR_BOTH, maxSpeedWheels * 0.2);
    tacho_set_position_sp(MOTOR_BOTH, distance*(360/17));
    tacho_run_to_rel_pos(MOTOR_BOTH);
    tacho_stop(MOTOR_BOTH);
}

void moveBackward(int distance)
{
    // Run motor 1 and 2 backwards at the same speed
    // Keep running until the "distance" has been met

    tacho_set_speed_sp(MOTOR_BOTH, -(maxSpeedWheels * 0.2));
    tacho_run_forever(MOTOR_BOTH);
    Sleep(distance);
    tacho_stop(MOTOR_BOTH);
}

void rotate(int degrees)
{
    // Run the motors in oppostite directions
    // negative "degrees" means left, positive means right

    tacho_set_speed_sp(MOTOR_LEFT, maxSpeedWheels * 0.2);
    tacho_set_speed_sp(MOTOR_RIGHT, -(maxSpeedWheels * 0.2));
    tacho_run_forever(MOTOR_LEFT);
    tacho_run_forever(MOTOR_RIGHT);
    Sleep(degrees);
    tacho_stop(MOTOR_LEFT);
    tacho_stop(MOTOR_RIGHT);
}
///////////////////////////////////////////////////////////////////////

//////////////////////// Initialization //////////////////////////////////
void initEverything()
{
    if (!brick_init())  // instantiates brick and allows remote-connection
	{
		return (1);
	}
    motorsInit();
}

void unInitEverything()
{
    brick_uninit();
}

void motorsInit()
{
    if (tacho_is_plugged(MOTOR_BOTH, TACHO_TYPE__NONE_))    // Checks if motors are connected to the "bricks" ports that are supposed to be used
    {  
        maxSpeedCenter = tacho_get_max_speed(MOTOR_CENTER, 0);   // Checks the max speed of the small motor and assigns it to global variable
        if (tacho_get_max_speed(MOTOR_LEFT, 0) < tacho_get_max_speed(MOTOR_RIGHT, 0))
        {
            maxSpeedWheels = tacho_get_max_speed(MOTOR_LEFT, 0); 
        }
        else
        {
            maxSpeedWheels = tacho_get_max_speed(MOTOR_RIGHT, 0); 
        }
        
        tacho_reset(MOTOR_BOTH);
        printf("Motors Initialized\n");
    }
    else 
    {
        printf( "Insert left motor to port A,\n"
        "Insert right motor to port D.\n"
        );
        Sleep(5000);
        brick_uninit();
        return;
    }
}
//////////////////////////////////////////////////////////////////////////