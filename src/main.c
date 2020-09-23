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
#define MOTOR_C    		OUTC
#define MOTOR_RIGHT    	OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_GYRO		IN2
#define SENSOR_SCANER	IN3
#define SENSOR_4		IN4
#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) // Definition used to refer to both ports for the EV3:s wheel motors at the same time.
//////////////////////////////////////////////////////////////////////////

///////////////// Global variables ///////////////////////////////////////
int maxSpeedWheels;         
int maxSpeedCenter;
// Touch
POOL_T touchSensor; 
int TouchReturnValue = 0;
// Gyro
POOL_T gyroSensor;
int rotationReference = 0;
int degrees;
// Sonic
POOL_T ultraSonic;
int sonicSensor;
// scaner
POOL_T scaner;
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
    if(!brick_init())
	{
		return (1);
	}
	rotationReference = motorSensorInit();



    brick_uninit();
	printf( "dying...\n" );
    return ( 0 );
}
//////////////////////////////////////////////////////////////////////////

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
        maxSpeedCenter = tacho_get_max_speed(MOTOR_FRONT, 0);   // Checks the max speed of the small motor and assigns it to global variable
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

int motorSensorInit(){
	
	if ( !brick_init()) return ( 1 ); /*Instansierar klossen och tillåter remote-connection*/
	printf( "*** ( EV3 ) Hello! ***\n" );
	Sleep(2000);

	if(!sensor_is_plugged((SENSOR_TOUCH|SENSOR_GYRO), SENSOR_TYPE__NONE_)) {
		printf("Stoppa in sensorer i port 1 och 2\n");
		brick_uninit();
		return(0);
	}
	/*Registrerar en sensor på sensor variabeln*/
	touchSensor = sensor_search(LEGO_EV3_TOUCH);
	gyroSensor = sensor_search(LEGO_EV3_GYRO);
	ultraSonic = sensor_search(LEGO_EV3_US); // Kolla med gustav
	
	//registerar sensor mode
	sensor_set_mode(gyroSensor, LEGO_EV3_GYRO_GYRO_G_AND_A);
	touch_set_mode_touch(touchSensor);
		
	rotationReference = sensor_get_value(0, gyroSensor, 0);// hämtar värde

	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
        maxSpeedWheels = tacho_get_max_speed( MOTOR_LEFT, 0 );	/*Kollar max hastigheten som motorn kan ha*/
        maxSpeedCenter = tacho_get_max_speed( MOTOR_FRONT, 0 );
		tacho_reset( MOTOR_BOTH );
		tacho_reset( MOTOR_FRONT );
    } else {
        printf( "Stoppa in vänstra motorn i port D,\n"
        "Stoppa in högra motorn i port A.\n"
        );
      	brick_uninit();
        return ( 0 );  /* Stänger av sig om motorer ej är inkopplade */
    }
	
	if ( tacho_is_plugged( MOTOR_FRONT, TACHO_TYPE__NONE_ )) {  
        maxSpeedCenter = tacho_get_max_speed( MOTOR_FRONT, 0 );	
        tacho_reset( MOTOR_FRONT );
    } else {
        printf( "Stoppa in vänstra motorn i port A,\n"
        "Stoppa in högra motorn i port B.\n"
        );
      	brick_uninit();
        return ( 0 );  /* Stänger av sig om motorer ej är inkopplade */
    }
	return rotationReference;
}
//////////////////////////////////////////////////////////////////////////

////////////////// Movement //////////////////////////////////////////////
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
    tacho_set_position_sp(MOTOR_BOTH, distance*(360/17));
    tacho_run_to_rel_pos(MOTOR_BOTH);
    tacho_stop(MOTOR_BOTH);
}

void moveUltraSonic(int wallDistance)
{
	// variabel definitioner
	sonicSensor= sensor_get_value(0,ultraSonic,0);
	int difference;
	
	while(sonicSensor==wallDistance)
	{
		// beraknar rorelse avstand 
		difference= sonicSensor - wallDistance;
		
		if (difference >= 0)
		{
			// langre bort fran vaggen
			moveForward(difference);	
		}
		else
		{
			// narmare till vaggen
			moveBackward(difference*(-1));
		}
		// updaterar avstand
		sonicSensor= sensor_get_value(0,ultraSonic,0);
	}
	return;
}

int rotate(int rotationReference, int degrees)
{
	// svänga höger eller vänster
	if (degrees>0)
	{
		// startar motoren
		tacho_set_speed_sp(MOTOR_RIGHT, maxSpeedWheels * -0.2);
		tacho_run_forever(MOTOR_RIGHT);
		// kontrollerar vinkel
		while(rotationReference <= degrees)
		{
			rotationReference = sensor_get_value(0,gyroSensor,0);
		}
		tacho_stop(MOTOR_RIGHT);
	}
	else
	{
		// startar motoren
		tacho_set_speed_sp(MOTOR_LEFT, maxSpeedWheels * 0.2);
		tacho_run_forever(MOTOR_LEFT);
		// kontrollerar vinkel
		while(rotationReference >= degrees)
		{
			rotationReference = sensor_get_value(0,gyroSensor,0);
		}
		tacho_stop(MOTOR_LEFT);	
	}
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