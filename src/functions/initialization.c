#include "common.h"

void initEverything()
{
    if (!brick_init())  // Instansierar klossen och tillåter remote-connection
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
    if (tacho_is_plugged(MOTOR_BOTH, TACHO_TYPE__NONE_))    // TACHO_TYPE__NONE_ = Alla typer av motorer
    {  
        maxSpeedCenter = tacho_get_max_speed(MOTOR_CENTER, 0);   // Kollar max hastigheten som motorn kan ha
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
        printf( "Stoppa in vänstra motorn i port A,\n"
        "Stoppa in högra motorn i port D.\n"
        );
        Sleep(5000);
        brick_uninit();
        return;
    }
}