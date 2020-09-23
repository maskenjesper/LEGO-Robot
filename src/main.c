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
#define MOTOR_WHEELS    (MOTOR_LEFT|MOTOR_RIGHT) // Definition used to refer to both ports for the EV3:s wheel motors at the same time.
#define MOTOR_ALL       (MOTOR_FRONT|MOTOR_LEFT|MOTOR_RIGHT)
//////////////////////////////////////////////////////////////////////////

///////////////// Global variables ///////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

/////////////// Function Declarations ////////////////////////////////////
// Initialization
void initEverything();
void unInitEverything();
void motorsInit();
int motorSensorInit();
// Movement
void moveForward(int distance); // Distance in cm
void moveBackward(int distance); // Distance in cm
void moveUltraSonic(int wallDistance);
void rotate(int rotationReference, int degrees);
// Actions
void releaseBook();
void findWall(int gyro);
//////////////////////////////////////////////////////////////////////////

////////////////////// Main //////////////////////////////////////////////
int main()
{
    initEverything();

	moveForward(10);
	moveBackward(10);
	rotate(360);
	releaseBook();

    unInitEverything();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////// Initialization //////////////////////////////////
void initEverything()
{
    brickInit();
    motorsInit();
    initSensors();
}

void unInitEverything()
{
    brick_uninit();
}

void brickInit()
{
    if (!brick_init())  // instantiates brick and allows remote-connection
	{
		return (1);
	}
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

void initSensors()
{
    if(!sensor_is_plugged((SENSOR_TOUCH|SENSOR_GYRO), SENSOR_TYPE__NONE_)) {
		printf("Stoppa in sensorer i port 1 och 2\n");
		brick_uninit();
		return(0);
	}

    sensor_set_mode(SENSOR_GYRO, LEGO_EV3_GYRO_GYRO_G_AND_A);
	touch_set_mode_touch(SENSOR_TOUCH);
}
//////////////////////////////////////////////////////////////////////////

////////////////// Movement //////////////////////////////////////////////
void moveForward(int distance)
{
    tacho_set_speed_sp(MOTOR_WHEELS, maxSpeedWheels * 0.2);
    tacho_set_position_sp(MOTOR_WHEELS, distance*(360/17));
    tacho_run_to_rel_pos(MOTOR_WHEELS);
    tacho_stop(MOTOR_WHEELS);
}

void moveBackward(int distance)
{
    tacho_set_speed_sp(MOTOR_WHEELS, -(maxSpeedWheels * 0.2));
    tacho_set_position_sp(MOTOR_WHEELS, distance*(360/17));
    tacho_run_to_rel_pos(MOTOR_WHEELS);
    tacho_stop(MOTOR_WHEELS);
}

void movePerpendicularToWall(int desiredDistanceFromWall)
{
	int distanceToMove = sensor_get_value(0,SENSOR_SCANNER,0) - desiredDistanceFromWall;

    while(!(desiredDistanceFromWall == sensor_get_value(0,SENSOR_SCANNER,0)))
    {
        if (distanceToMove > 0)
        {
            moveForward(distanceToMove);
        }
        else
        {
            moveBackward(distanceToMove);
        } 
    }
}

void rotate(int degrees)
{
	int rotationTarget = sensor_get_value(0,SENSOR_GYRO,0) + degrees;

	while(!(sensor_get_value(0,SENSOR_GYRO,0) == rotationTarget)
	{
		tacho_set_speed_sp(MOTOR_RIGHT, -200);
		tacho_set_speed_sp(MOTOR_LEFT, 200);
		tacho_run_forever(MOTOR_WHEELS);
	}
	tacho_stop(MOTOR_WHEELS);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////// Actions /////////////////////////////////
void releaseBook()
{
	tacho_set_speed_sp(MOTOR_FRONT, maxSpeedCenter * -1 );
	tacho_run_forever(MOTOR_FRONT);
	Sleep(4500);
	tacho_stop(MOTOR_FRONT);
	
	tacho_set_speed_sp(MOTOR_FRONT, maxSpeedCenter );
	tacho_run_forever(MOTOR_FRONT);
	Sleep(4500);
	tacho_stop(MOTOR_FRONT);
}

void findWall(int gyro)
{
int a;
int b;
int c;
int d;

// roterar med både hjulnano
tacho_set_speed_sp(MOTOR_RIGHT, maxSpeedWheels * -0.2);
tacho_run_forever(MOTOR_RIGHT);
tacho_set_speed_sp(MOTOR_LEFT, maxSpeedWheels * 0.2);
tacho_run_forever(MOTOR_LEFT);

a = sensor_get_value( 0,ultraSonic,0 );
rotate(90);
Sleep(1000);
b = sensor_get_value( 0,ultraSonic,0 )
rotate(90);
Sleep(1000);
c = sensor_get_value( 0,ultraSonic,0 )
rotate(90);
Sleep(1000);
d = sensor_get_value( 0,ultraSonic,0 )
rotate(90);
Sleep(1000);
}
//////////////////////////////////////////////////////////////////////////