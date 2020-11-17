// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#define MaxFileLength 32
#define MaxStringLength 255

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// User2System function:
// Input: - user space address (int)
//        - Limit of buffer (int)
// Output: - Buffer (char*)
// Purpose: Copy buffer from user memory space to System memory space.

char* User2System(int virtAddr, int limit) {
  int i;
  int oneChar;
  char* kernelBuf = NULL;
  kernelBuf = new char[limit + 1]; // Why is limit + 1 ?

  if (kernelBuf == NULL) {
    return kernelBuf;
  }

  memset(kernelBuf, 0, limit + 1);
    
  for(i = 0; i < limit; i++) {
    machine->ReadMem(virtAddr+i, 1, &oneChar);
    kernelBuf[i] = (char)oneChar;

    if(oneChar == 0) {
      break;
    }
  }
  return kernelBuf;
}


// System2User function:
// Input: - User space address (int)
//        - Limit buffer (int)
//        - Buffer (char[])
// Output: - Number of file copied (int)
// Purpose: Copy buffer from system memory space to user memory space.

int System2User(int virtAddr, int len, char* buffer) {
  if(len < 0) {
    return -1;
  } 

  if(len == 0) {
    return len;
  }

  int i = 0;
  int oneChar = 0;
  do {
    oneChar = (int)buffer[i];
    machine->WriteMem(virtAddr + i, 1, oneChar);
    i++;
  }while(i < len && oneChar != 0);
  return i;
}


// Using this instead Halt() funtion:
void IncreasePC()
{
    // Get the current PC value:
    int PCRegValue = machine->ReadRegister(PCReg);

    // Change the Previous PC value to current PC value:
    machine->WriteRegister(PrevPCReg, PCRegValue);

    // Get the next PC value:
    int NextPCRegValue = machine->ReadRegister(NextPCReg);

    // Change the current PC Reg value to next PC value:
    machine->WriteRegister(PCReg, NextPCRegValue);

    // Inscrease the next PC Reg value 4 bytes:
    machine->WriteRegister(NextPCReg, NextPCRegValue + 4);
}


void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {
      case NoException:
        return;

      case SyscallException: {
        switch (type) {
          case SC_Halt: {
            DEBUG('a', "\n Shutdown, initiated by user program.");
            printf("\n\n Shutdown, initiated by user program.");
            interrupt->Halt();
            break;
          }
          case SC_Exit: {
            break;
          }
          case SC_Exec: {
            break;
          }
          case SC_Join: {
            break;
          }

          case SC_Create: {
              int virtAddr;
              char* filename;

              DEBUG('a', "\n SC_Create call ...");
              DEBUG('a', "\n Reading virtual address of filename");

              // Get the given filename form register 4:
              virtAddr = machine->ReadRegister(4);
              DEBUG('a', "\n Reading filename.");

              // Max filename is 32
              filename = User2System(virtAddr, MaxFileLength + 1);
              if(filename == NULL) {
                printf("\n Not enough memory in system");
                DEBUG('a', "\n Not enough memory in system");

                // Return Error status -1
                machine->WriteRegister(2, -1);
                delete filename;
                return;
              }
              DEBUG('a', "\n Finish reading filename.");

              // Using fileSystem Object of OpenFile class to create file.
              if(!fileSystem->Create(filename, 0)) {
                printf("\n Error create file '%s'", filename);
                machine->WriteRegister(2, -1);
                delete filename;
                return;
              }
              machine->WriteRegister(2, 0);
              delete filename;
              break;
            }
          case SC_Open: {
            break;
          }
          case SC_Read: {
            break;
          }
          case SC_Write: {
            break;
          }
          case SC_Close: {
            break;
          }
          case SC_Fork: {
            break;
          }
          case SC_Yield: {
            break;
          }
          case SC_Sub: {
            int op1 = machine->ReadRegister(4);
            int op2 = machine->ReadRegister(5);
            int result = op1 - op2;
            machine->WriteRegister(2, result);
            IncreasePC();
            break;
          }

	  case  SC_ReadInt:{
	  //doc mot so int va tra ve.
            DEBUG('a', "Read integer number from console.\n");
            int number = 0; // luu ket qua tra ve
	    int nDigit = 0;
	    int i;
            int MAX_INT_LENGTH= 255;
	    char* bufer = new char[MAX_INT_LENGTH+1];
	    nDigit = gSynchConsole->Read(bufer, MAX_INT_LENGTH);// doc buffer va tra ve ki tu doc duoc
	    i = bufer[0] == '-' ? 1:0 ;
	    for (; i < nDigit; ++i) 
	    {
		if(bufer[i]== '.' )
		{
			int j=i+1;
			for( ; j < nDigit; j++)
			{
				if(bufer[j] != '0')
				{ 
				        machine->WriteRegister(2, 0);
					delete buffer;
                                        return ;
				}
			}
		}
			
	    }
	    //chuyen chuoi ve so nguyen
	    for (; i < nDigit; ++i) 
	    {
		number = number*10 + (int) (bufer[i] - 48);
	    }
	    number = bufer[0] == '-' ? -1*number : number;
	    machine->WriteRegister(2, number);
	    delete bufer;
	  }  

	  case SC_PrintInt:{
	    char s[255], neg, tmp;
	    neg = '-';
	    int i, n, mid, sz;
	    i = n = 0;
	    DEBUG('a', "Read argument value at r4");
	    n = machine->ReadRegister(4);
	    //chuyen so am thanh so duong
	    if (n < 0)
		{
			gSynchConsole->Write(&neg,1);
			n = -n;
		}
	    do {
			s[i++] = n%10 + '0';
		} while (( n /= 10) > 0);
	    sz = i;
            s[sz] = '\n';
	    mid = i / 2;
	   // o tren do..while: ki tu cuoi luu o dau 
	   // duoi day while: dao nguoc lai
	    while (i-->mid)
	      {
		tmp = s[sz-i-1];
		s[sz-i-1] = s[i];
		s[i] = tmp;
	      }
	   gSynchConsole->Write(s, sz);
	   }
	     break;


          // ReadString Syscall:
          case SC_ReadString: {
            int virtAddr, length;

            // Get buffer (char[]) argument from Register 4:
            virtAddr = machine->ReadRegister(4);

            // Get max length (int) argumrnt from Register 5:
            length = machine->ReadRegister(5);

            // Create a system buffer in kernal mode with the same max length:
            char* systemBuffer;
            systemBuffer = User2System(virtAddr, length);

            // Reading give String from console and store it into systemBuffer:
            gSynchConsole->Read(systemBuffer, length);

            // copy the system buffer to the user buffer of user mode:
            System2User(virtAddr, length, systemBuffer);
            delete systemBuffer;

            // Increase PC:
            IncreasePC();
            break;
          }

          // Print String Syscall:
          case SC_PrintString: {
            // Get the buffer address store the string that need to be print:
            int virtAddr;
            virtAddr = machine->ReadRegister(4);

            // Create buffer in Kernel mode by clonning the given String in
            // usermode: 
            char* kernalBuffer;
            kernalBuffer = User2System(virtAddr, MaxStringLength);

            // Calc the real length of the given String: 
            int length = 0;
            while (kernalBuffer[length] != 0) {
              length++;
            }

            // Print the given String to screen by using SynchConsole class:
            gSynchConsole->Write(kernalBuffer, length + 1);
            delete kernalBuffer;

            // Increase PC:
            IncreasePC();
            break;
          }


	  case SC_ReadChar:
	  {
	  	DEBUG('a', "Read Char Syscall ...\n");
		char *buff = new char[2]; 
		int numbOfChar = gSynchConsole->Read(buff, 2);
		if (numbOfChar == 0)
		{
			DEBUG('a', "Empty character.\n");
			machine->WriteRegister(2, 0);						
		}
		else
			if (numbOfChar == 1)
			{						
				DEBUG('a', "Reading Character ...\n");
				char readChar = buff[0];
				machine->WriteRegister(2, readChar);						
			}
			else
			{
				DEBUG('a', "Only the first character will be read.\n");
				char readChar = buff[0];
				machine->WriteRegister(2, readChar);
			}
		delete buff;
		IncreasePC();
		break;
	  }
	  case SC_PrintChar:
	  {
		DEBUG('a', "Print Char Syscall\n");	
		char printChar;
		printChar = (char)machine->ReadRegister(4);
		gSynchConsole->Write(&printChar, 1);				
		break;	
	  }
          default: {
            printf("\n Unexpected user mode exception (%d %d)", which, type);
            interrupt->Halt();
          }
       }
       break;
      }

      case PageFaultException: {
        DEBUG('a', "\n This is PageFaultException.");
        printf("\n\n This is PageFaultException.");
        interrupt->Halt();
        break;
      }
      case ReadOnlyException: {
        DEBUG('a', "\n This is ReadOnlyException.");
        printf("\n\n This is ReadOnlyException.");
        interrupt->Halt();
        break;
      }
      case BusErrorException: {
        DEBUG('a', "\n This is BusErrorException.");
        printf("\n\n This is BusErrorException.");
        interrupt->Halt();
        break;
      }
      case AddressErrorException: {
        DEBUG('a', "\n This is AddressErrorException.");
        printf("\n\n This is AddressErrorException.");
        interrupt->Halt();
        break;
      }
      case OverflowException: {
        DEBUG('a', "\n This is OverflowException.");
        printf("\n\n This is OverflowException.");
        interrupt->Halt();
        break;
      }
      case IllegalInstrException: {
        DEBUG('a', "\n This is IllegalInstrException.");
        printf("\n\n This is IllegalInstrException.");
        interrupt->Halt();
        break;
      }
      case NumExceptionTypes: {
        DEBUG('a', "\n This is NumExceptionTypes.");
        printf("\n\n This is NumExceptionTypes.");
        interrupt->Halt();
        break;
      }
      /*case InvalidDataException: {
         DEBUG('a', "\n This is InvalidDataException.");
         printf("\n\n This is InvalidDataException.");
         interrupt->Halt();
         break;
      }*/
    }
}
