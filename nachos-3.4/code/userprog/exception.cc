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




void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {
      case NoException:
        return;

      case SyscallException:
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
            interrupt->Halt();
          }
          default: {
            printf("\n Unexpected user mode exception (%d %d)", which, type);
            interrupt->Halt();
          }
       }
      case PageFaultException: {
        DEBUG('a', "\n This is PageFaultException.");
        printf("This is PageFaultException.");
        interrupt->Halt();
        break;
      }
      case ReadOnlyException: {
        DEBUG('a', "\n This is ReadOnlyException.");
        printf("This is ReadOnlyException.");
        interrupt->Halt();
        break;
      }
      case BusErrorException: {
        DEBUG('a', "\n This is BusErrorException.");
        printf("This is BusErrorException.");
        interrupt->Halt();
        break;
      }
      case AddressErrorException: {
        DEBUG('a', "\n This is AddressErrorException.");
        printf("This is AddressErrorException.");
        interrupt->Halt();
        break;
      }
      case OverflowException: {
        DEBUG('a', "\n This is OverflowException.");
        printf("This is OverflowException.");
        interrupt->Halt();
        break;
      }
      case IllegalInstrException: {
        DEBUG('a', "\n This is IllegalInstrException.");
        printf("This is IllegalInstrException.");
        interrupt->Halt();
        break;
      }
      case NumExceptionTypes: {
        DEBUG('a', "\n This is NumExceptionTypes.");
        printf("This is NumExceptionTypes.");
        interrupt->Halt();
        break;
      }
    }
}
