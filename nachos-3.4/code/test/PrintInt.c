#include "syscall.h"
#include "copyright.h"
int main()
{
	int n;
	PrintString("Nhap 1 so:");
	n = ReadInt();
	PrintString("So vua nhap:");
	PrintInt(n); 
	return 0;
}
