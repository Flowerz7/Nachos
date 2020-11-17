/* print readable character in ascii table
 */

#include "syscall.h"
#include "copyright.h"

int
main()
{
    int octCode = 32;
    printf("\n%5d%5c", octCode, octCode);
    return 0;
}
