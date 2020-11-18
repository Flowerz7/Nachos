/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"


int
main()
{
    int decCode = 32;
    int i = decCode;
    char asciiChar;
    for (i; i < 128; i++) {
	    asciiChar = '0' + i - '0'; //chuyen kieu int sang char
        PrintInt(i);
	    PrintChar(' ');
        PrintChar(asciiChar);
        PrintChar('\n');
    }

    return 0;
}
