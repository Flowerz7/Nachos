/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"


int
main()
{
    int octCode = 32;
    int i = octCode;
    for (i; i < 128; i++) {
        /*printf("\n%5d%5c", octCode, octCode);*/
        PrintInt(octCode);
	    PrintString(' ');
        PrintChar(octCode);
        PrintString('\n');
        
    }

    return 0;
}
