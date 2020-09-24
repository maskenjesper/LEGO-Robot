///////////////////// Includes ///////////////////////////////////////////
#include <stdio.h>
#include "brick.h"
#include <unistd.h>
//////////////////////////////////////////////////////////////////////////

////////////////// General definitions ///////////////////////////////////
#define Sleep( msec ) usleep(( msec ) * 1000 ) // Defines Sleep() to pause execution of the program for given amount of time in milliseconds.
//////////////////////////////////////////////////////////////////////////

////////////////// Physical definitions //////////////////////////////////
#define MOTOR_LEFT    	OUTA
#define MOTOR_FRONT   	OUTB
#define MOTOR_RIGHT    	OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_GYRO		IN2
#define SENSOR_SCANNER	IN3
#define MOTOR_BOTH      (MOTOR_LEFT|MOTOR_RIGHT) // Definition used to refer to both ports for the EV3:s wheel motors at the same time.
#define MOTOR_ALL       (MOTOR_FRONT|MOTOR_LEFT|MOTOR_RIGHT)
//////////////////////////////////////////////////////////////////////////

///////////////// Global variables ///////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

/////////////// Function Declarations ////////////////////////////////////
// Initialization
void initEverything();
void unInitEverything();
//void brickInit();
void motorsInit();
//void initSensors()
// Movement
void moveForward(int distance); // Distance in cm
void moveBackward(int distance); // Distance in cm
void moveTowardsWallAndStop(int desiredDistanceFromWall);
void rotate(int degrees);
// Actions
void releaseBook();
void alignParallelWithWall();
//////////////////////////////////////////////////////////////////////////

////////////////////// Main //////////////////////////////////////////////
int main()
{
    if (!brick_init())  // instantiates brick and allows remote-connection
	{
        printf("Brick did not initialize");
		return 0;
	}
    initEverything();

    rotate(90);

    unInitEverything();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////// Initialization //////////////////////////////////
void initEverything()
{
    motorsInit();
}

void unInitEverything()
{
    printf("Program stopped, uninitializing");
    Sleep(10000);
    brick_uninit();
}

void motorsInit()
{
    if (tacho_is_plugged(MOTOR_ALL, TACHO_TYPE__NONE_))    // Checks if motors are connected to the "bricks" ports that are supposed to be used
    {  
        tacho_reset(MOTOR_ALL);
        printf("Tachos Initialized\n");
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

////////////////// Movement //////////////////////////////////////////////
void moveForward(int distance)
{   
    int targetPosition = 1.05*(tacho_get_position(MOTOR_LEFT, targetPosition) + distance*20);
    tacho_set_speed_sp(MOTOR_BOTH, 200);

    while(true)
    {
        if(targetPosition > tacho_get_position(MOTOR_LEFT, targetPosition))
        {
            tacho_run_forever(MOTOR_BOTH);
        }
        else
        {
            tacho_stop(MOTOR_BOTH);
            return;
        }
    }
}

void moveBackward(int distance)
{
    int targetPosition = 1*(tacho_get_position(MOTOR_LEFT, targetPosition) - distance*20);
    tacho_set_speed_sp(MOTOR_BOTH, -200);

    while(true)
    {
        if(targetPosition < tacho_get_position(MOTOR_LEFT, targetPosition))
        {
            tacho_run_forever(MOTOR_BOTH);
        }
        else
        {
            tacho_stop(MOTOR_BOTH);
            return;
        }
    }
}

void moveTowardsWallAndStop(int targetDistanceFromWall)
{
    tacho_set_speed_sp(MOTOR_BOTH, 200);
    while(true)
    {
        if(targetDistanceFromWall < (sensor_get_value(0,SENSOR_SCANNER,0))/10)
        {
            tacho_run_forever(MOTOR_BOTH);
        }
        else
        {
            tacho_stop(MOTOR_BOTH);
            return;
        } 
    }
}

void rotate(int degrees)
{
	int rotationTarget = sensor_get_value(0,SENSOR_GYRO,0) + degrees;

	tacho_set_speed_sp(MOTOR_RIGHT, -200);
	tacho_set_speed_sp(MOTOR_LEFT, 200);
	while(!(sensor_get_value(0,SENSOR_GYRO,0) == rotationTarget)
	{
		tacho_run_forever(MOTOR_BOTH);
	}
	tacho_stop(MOTOR_BOTH);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////// Actions /////////////////////////////////
void releaseBook()
{
	tacho_set_speed_sp(MOTOR_FRONT, -1000);
	tacho_run_forever(MOTOR_FRONT);
	Sleep(4500);
	tacho_stop(MOTOR_FRONT);
	
	tacho_set_speed_sp(MOTOR_FRONT, 1000);
	tacho_run_forever(MOTOR_FRONT);
	Sleep(4500);
	tacho_stop(MOTOR_FRONT);
}

void alignParallelWithWall()
{
    int shortestDistanceToWall = 500, respectiveRotation;

    for(int degree = 0; degree < 360; degree++)
    {
        if(shortestDistanceToWall > sensor_get_value(0,SENSOR_SCANNER,0))
        {
            shortestDistanceToWall = sensor_get_value(0,SENSOR_SCANNER,0);
            respectiveRotation = degree;
        }
        rotate(1);
    }

    tacho_set_speed_sp(MOTOR_RIGHT, -200);
    tacho_set_speed_sp(MOTOR_LEFT, 200);
    while(respectiveRotation + 90 != sensor_get_value(0,gyroSensor,0))
    {
		tacho_run_forever(MOTOR_BOTH);
    }
	tacho_stop(MOTOR_BOTH);
}
//////////////////////////////////////////////////////////////////////////