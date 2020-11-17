/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"


int octCode = 32;
int i = octCode;

int
main()
{
    for (i; i < 128; i++) {
        /*printf("\n%5d%5c", octCode, octCode);*/
        PrintInt(octCode);
	    PrintChar(' ');
        PrintChar(octCode);
        PrintChar('\n');
        
    }

    return 0;
}
