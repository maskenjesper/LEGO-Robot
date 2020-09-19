void moveForward(int distance)
{
    // Run motor 1 and 2 forwards at the same speed
    // Keep running until the "distance" has been met

    int TURN_PER_SECOND = 21600;  
    int CENTIMETERS_PER_SECOND = 1228;    // Unit to be used for speed
    int speed = 1;    // Unit is CentimeterPerSecond
    int duration = 1000 * (distance / speed);   // Duration to spin the wheels in MilliSeconds
 
    tacho_set_speed_sp(MOTOR_BOTH, speed * CENTIMETERS_PER_SECOND);
    tacho_run_forever(MOTOR_BOTH);
    Sleep(duration);
    tacho_stop(MOTOR_BOTH);
}