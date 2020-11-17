/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"

int
main()
{
    int octCode = 32;
    for (int i = octCode; i < 128; i++) {
        /*printf("\n%5d%5c", octCode, octCode);*/
        PrintInt(octCode);
	    PrintChar(' ');
        PrintChar(octCode);
        PrintChar('\n');
        
    }

    return 0;
}
