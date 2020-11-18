/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"


int
main()
{
    int decCode = 32;
    char asciiChar;
    for (decCode; decCode < 128; decCode++) {
	    asciiChar = '0' + decCode - '0'; //chuyen kieu int sang char
        PrintInt(decCode);
	    PrintChar(' ');
        PrintChar(asciiChar);
        PrintChar('\n');
    }

    Halt();
    return 0;
}
