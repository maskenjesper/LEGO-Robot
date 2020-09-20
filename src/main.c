#include "movement.h"
#include "initialization.h"

void releaseBook();

int main()
{
    initEverything();

    moveForward(5000);
    moveBackward(5000);
    rotate(5000);

    unInitEverything();

    return 0;
}