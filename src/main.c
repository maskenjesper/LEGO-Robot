#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include "movement.h"

#define Sleep( msec ) usleep(( msec ) * 1000 ) // Definerar sleep där Sleep(1000)= 1 sekund

#define MOTOR_LEFT    	OUTA
#define MOTOR_CENTER   	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_RIGHT    	OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_2		IN2
#define SENSOR_3		IN3
#define SENSOR_4		IN4

#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) // Mask används för att kunna hänvisa till båda på samma gång

void releaseBook();

// Globala variabler
int maxSpeedCenter;
int maxSpeedWheels;

int main()
{
    if (!brick_init())  // Instansierar klossen och tillåter remote-connection
	{
		return ( 1 );
	}
    motorsInit();
	Sleep(5000);

    ///////////////////////////////////////////////////////////////////////

    moveForward(5000);
    moveBackward(5000);
    rotate(5000);

    ///////////////////////////////////////////////////////////////////////

    brick_uninit();
	printf( "dying...\n" );
    Sleep(5000);

    return 0;
}