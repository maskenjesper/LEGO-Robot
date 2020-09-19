#include "movement.h"

void releaseBook();

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