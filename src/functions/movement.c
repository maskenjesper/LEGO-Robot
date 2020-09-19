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

void moveForward(int distance)
{
    // Run motor 1 and 2 forwards at the same speed
    // Keep running until the "distance" has been met

    tacho_set_speed_sp(MOTOR_BOTH, maxSpeedWheels * 0.2);
    tacho_run_forever(MOTOR_BOTH);
    Sleep(distance);
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