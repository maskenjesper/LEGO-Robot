#include "common.h"

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