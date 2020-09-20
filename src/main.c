#include "movement.h"
#include "initialization.h"

void releaseBook();

int main()
{
    if (!brick_init())  // Instansierar klossen och tillåter remote-connection
	{
		return (1);
	}
    motorsInit();

    moveForward(5000);
    moveBackward(5000);
    rotate(5000);

    brick_uninit();

    return 0;
}