/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"


int
main()
{
    int octCode = 32;
    int i = octCode;
    char asciiChar;
    for (i; i < 128; i++) {
	asciiChar = '0' + i - '0'; //chuyen kieu int sang char
        /*printf("\n%5d%5c", octCode, octCode);*/
        PrintInt(i);
	PrintChar(' ');
        PrintChar(asciiChar);
        PrintChar('\n');
    }

    return 0;
}
