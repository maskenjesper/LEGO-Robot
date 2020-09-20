#include "initialization.h"
#include "movement.h"
#include "actions.h"

int main()
{
    initEverything();

    moveForward(5000);
    moveBackward(5000);
    rotate(5000);

    unInitEverything();

    return 0;
}