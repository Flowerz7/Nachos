/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"
#include "copyright.h"

int A[1024];	/* size of physical memory; with code, we'll run out of space!*/

int
main()
{
    int i, j, tmp, n;
    PrintString("Nhap so n (n <= 100):");
    n = ReadInt();


    /* first initialize the array, in reverse sorted order */
    for (i = 0; i < n; i++)
    {
        PrintString("Nhap so cac phan tu mang:");
        A[i] = ReadInt();
    }

    /* then sort! */
    for (i = 0; i < n-1; i++)
        for (j = i; j < (n - 1 - i); j++)
	   if (A[j] > A[j + 1]) {	/* out of order -> need to swap ! */
	      tmp = A[j];
	      A[j] = A[j + 1];
	      A[j + 1] = tmp;
    	   }
    Exit(A[0]);		/* and then we're done -- should be 0! */
}
