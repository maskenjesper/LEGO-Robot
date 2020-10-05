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
#define SENSOR_TOUCH_LEFT	IN1
#define SENSOR_GYRO		    IN2
#define SENSOR_SCANNER	    IN3
#define SENSOR_TOUCH_RIGHT  IN4
#define MOTOR_BOTH      (MOTOR_LEFT|MOTOR_RIGHT) // Definition used to refer to both ports for the EV3:s wheel motors at the same time.
#define MOTOR_ALL       (MOTOR_FRONT|MOTOR_LEFT|MOTOR_RIGHT)
//////////////////////////////////////////////////////////////////////////

/////////////// Function Declarations ////////////////////////////////////
// Initialization
void initEverything();
void brickInit();
void motorsInit();
void unInitEverything();
// Movement
void moveForward(int distance); // Distance in cm
void moveBackward(int distance); // Distance in cm
void moveTowardsWallAndStop(int desiredDistanceFromWall);
void rotate(int degrees);
void rotateAbsolute(int absoluteRotation, int speed);
// Actions
void releaseBook();
void alignParallelWithWall(int side); // Parameter side as -1 for right,  1 for left and 0 for front.
int findWall(int degrees);
// Diagnostic
void printGyroValue();
void printUltrasoundValue();
//////////////////////////////////////////////////////////////////////////

////////////////////// Main //////////////////////////////////////////////
int main()
{
    initEverything();

    alignParallelWithWall(1);
    moveForward(250);
    rotate(90);
    moveTowardsWallAndStop(30);
    releaseBook();
    rotate(180);
    moveTowardsWallAndStop(50);
    rotate(-90);
    moveForward(250);

    unInitEverything();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////// Initialization //////////////////////////////////
void initEverything()
{
    brickInit();
    motorsInit();
}

void brickInit()
{
    if (!brick_init())  // instantiates brick and allows remote-connection
	{
        printf("\n\n\n Brick did not initialize\n");
        Sleep(10000);
	}
}

void motorsInit()
{
    if (tacho_is_plugged(MOTOR_ALL, TACHO_TYPE__NONE_))    // Checks if motors are connected to the "bricks" ports that are supposed to be used
    {  
        tacho_reset(MOTOR_ALL);
        printf("\n\n\n Tachos Initialized\n");
    }
    else 
    {
        printf( "\n\n\n Insert left motor to port A,\n"
        " Insert right motor to port D.\n"
        );
        Sleep(10000);
        brick_uninit();
        return;
    }
}

void unInitEverything()
{
    printf("\n\n\n Program stopped, uninitializing\n");
    Sleep(10000);
    brick_uninit();
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

    rotateAbsolute(rotationTarget, 200);
}

void rotateAbsolute(int absoluteRotation, int speed)
{
    if(speed < 50)
    {
        speed = 50;
    }

    if(absoluteRotation > sensor_get_value(0,SENSOR_GYRO,0))
    {
	    tacho_set_speed_sp(MOTOR_RIGHT, -speed);
	    tacho_set_speed_sp(MOTOR_LEFT, speed);

        while(sensor_get_value(0,SENSOR_GYRO,0) < absoluteRotation)
	    {
		    tacho_run_forever(MOTOR_BOTH);
	    }
	    tacho_stop(MOTOR_BOTH);
        Sleep(50);
        rotateAbsolute(absoluteRotation, speed/2);
    }
    else if(absoluteRotation < sensor_get_value(0,SENSOR_GYRO,0))
    {
        tacho_set_speed_sp(MOTOR_RIGHT, speed);
	    tacho_set_speed_sp(MOTOR_LEFT, -speed);

        while(sensor_get_value(0,SENSOR_GYRO,0) > absoluteRotation)
	    {
		    tacho_run_forever(MOTOR_BOTH);
	    }
	    tacho_stop(MOTOR_BOTH);
        Sleep(50);
        rotateAbsolute(absoluteRotation, speed/2);
    }
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////// Actions /////////////////////////////////
void releaseBook()
{
	tacho_set_speed_sp(MOTOR_FRONT, -1000);
	tacho_run_forever(MOTOR_FRONT);
	Sleep(4500);
	tacho_stop(MOTOR_FRONT);
	Sleep(2000);

	tacho_set_speed_sp(MOTOR_FRONT, 1000);
	tacho_run_forever(MOTOR_FRONT);
	Sleep(4500);
	tacho_stop(MOTOR_FRONT);
}

void alignParallelWithWall(int side)
{
    int targetRotation = findWall(360);
    rotateAbsolute(targetRotation + 180, 200);

    tacho_set_speed_sp(MOTOR_BOTH, -200);
    tacho_run_forever(MOTOR_BOTH);
    bool running = true;
    while(running)
    {
        if(sensor_get_value(0,SENSOR_TOUCH_LEFT,0) && sensor_get_value(0,SENSOR_TOUCH_RIGHT,0))
        {
            Sleep(3000);
            tacho_stop(MOTOR_BOTH);
            targetRotation = sensor_get_value(0,SENSOR_GYRO,0);
            running = false;
        }
    }

    moveForward(50);

    rotateAbsolute(targetRotation - (90*side), 100);
    Sleep(500);
}

int findWall(int degrees)
{
    int shortestDistanceToWall = 2550, targetRotation;
    int scanRotation = sensor_get_value(0,SENSOR_GYRO,0) + degrees;

    if(degrees > 0)
    {
        tacho_set_speed_sp(MOTOR_RIGHT, -100);
        tacho_set_speed_sp(MOTOR_LEFT, 100);
        tacho_run_forever(MOTOR_BOTH); 
        while(sensor_get_value(0,SENSOR_GYRO,0) < scanRotation)
        {
            if(sensor_get_value(0,SENSOR_SCANNER,0) < shortestDistanceToWall)
            {
                shortestDistanceToWall = sensor_get_value(0,SENSOR_SCANNER,0);
                targetRotation = sensor_get_value(0,SENSOR_GYRO,0);
            }
        }
    }
    else
    {
        tacho_set_speed_sp(MOTOR_RIGHT, 100);
	    tacho_set_speed_sp(MOTOR_LEFT, -100);
        tacho_run_forever(MOTOR_BOTH); 
        while(sensor_get_value(0,SENSOR_GYRO,0) > scanRotation)
        {
            if(sensor_get_value(0,SENSOR_SCANNER,0) < shortestDistanceToWall)
            {
                shortestDistanceToWall = sensor_get_value(0,SENSOR_SCANNER,0);
                targetRotation = sensor_get_value(0,SENSOR_GYRO,0);
            }
        }
    }
    
    tacho_stop(MOTOR_BOTH);
    Sleep(500);
    return targetRotation;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////// Diagnostic //////////////////////////////////
void printGyroValue()
{
    int gyro_value = sensor_get_value(0,SENSOR_GYRO,0);
    Sleep(100);
    printf("%d\n", gyro_value);
}

void printUltrasoundValue()
{
    int ultrasound_value = sensor_get_value(0,SENSOR_SCANNER,0);
    Sleep(100);
    printf("%d\n", ultrasound_value);
}
//////////////////////////////////////////////////////////////////////////