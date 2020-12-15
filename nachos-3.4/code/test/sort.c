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

int A[100];	/* maximum size available*/

int
main()
{
    int i, j, tmp, n, asc;
    do
    {
        PrintString("Nhap so n (0 < n <= 100):");
        n = ReadInt();
        if (n > 100 || n < 0)
        {
            PrintString("Invalid data\n");
        }
    } while (n > 100 || n < 0);
    

    /*initialize the array*/
    for (i = 0; i < n; i++)
    {
        PrintString("Nhap cac phan tu mang:");
        A[i] = ReadInt();
    }

    PrintString("Nhap thu tu sap xep (tang dan: 1, giam dan: 0):");
    asc = ReadInt();

    /*sort */
    if (asc == 1) {

        for (i = 0; i < n - 1; i++)
        {
            for (j = 0; j < (n - 1 - i); j++)
            {
                if (A[j] > A[j + 1]) {	/* out of order -> need to swap ! */
                    tmp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = tmp;
                }
            }
        }
    }
    else if (asc == 0) {
	  PrintChar('\n');
        for (i = 0; i < n - 1; i++)
        {
            for (j = 0; j < (n - 1 - i); j++)
            {
                if (A[j] < A[j + 1]) {	/* out of order -> need to swap ! */
                    tmp = A[j];
                    A[j] = A[j + 1];
                    A[j + 1] = tmp;
                }
            }
        }
    }
    else
    {
        PrintString("Invalid data\n");
        Halt();
    }
    PrintString("Mang da sap xep: ");
    for (i = 0; i < n; i++)
    {
        PrintInt(A[i]);
        PrintChar(' ');
    }
    Halt();
}
