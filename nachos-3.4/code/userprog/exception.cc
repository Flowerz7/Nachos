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

          // SpaceId Exec(char* name);
          // Input: the given file name of process.
          // Output: Return the id of process if exec suscess,
          //         Otherwise, return -1.
          case SC_Exec: {

            // Reading file name from R4 to store it into kernal mode:
            int virtAddr;
            virtAddr = machine->ReadRegister(4);  
            char* name;
            name = User2System(virtAddr, MaxFileLength + 1); 
  
            // Not enough memory in Kernal mode case:
            if(name == NULL) {
              DEBUG('a', "\n Not enough memory in System");
              printf("\n Not enough memory in System");

              machine->WriteRegister(2, -1);
              //IncreasePC();
              return;
            }

            // Opening given file name:
            OpenFile *oFile = fileSystem->Open(name);
            if (oFile == NULL) {
              printf("\nExec:: Can't open this file.");

              machine->WriteRegister(2,-1);
              IncreasePC();
              return;
            }

            delete oFile;

            // Return child process id:
            int id = processTab->ExecUpdate(name); 

            machine->WriteRegister(2,id);
            delete[] name;  
            IncreasePC();
            return;  
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

          case SC_ReadInt: {
            char* buffer;
            int MaxBufferSize = 255;
            buffer = new char[MaxBufferSize + 1];

            // Reading number from console and store it into buffer:
            // Store the number of byte into numBytes:
            int numOfByte = gSynchConsole->Read(buffer, MaxBufferSize);

            // firtNumIndex will store the first char of the valid substring
            // that can be converted to integer:
            int firstNumIndex = 0;
            
            // lastNumIndex will store the last char of the valid substring
            // that can be converted to integer:
            int lastNumIndex = 0;

            // Check the given number is positive or negative:
            bool isNegative = false; 
            if(buffer[0] == '-')
            {
              isNegative = true;
              firstNumIndex = 1;
              lastNumIndex = 1;
            }

            // Find the max length substring from the given string that can be
            // convert to integer:
            for(int i = firstNumIndex; i < numOfByte; i++)
            {
              // Check Case 1: If have . in String => All digit after . is 0:
              if(buffer[i] == '.') 
              {
                int j = i + 1;
                for(; j < numOfByte; j++)
                {
                  if(buffer[j] != '0')
                  {
                    printf("\n\n The integer number is not valid");
                    DEBUG('a', "\n The integer number is not valid");
                    machine->WriteRegister(2, 0);
                    IncreasePC();
                    delete buffer;
                    return;
                  }
                }

                lastNumIndex = i - 1;
                break;
              }
              // Check Case 2: all digit is betwenn 0 and 9:
              else if(buffer[i] < '0' && buffer[i] > '9')
              {
                printf("\n\n The integer number is not valid");
                DEBUG('a', "\n The integer number is not valid");
                machine->WriteRegister(2, 0);
                IncreasePC();
                delete buffer;
                return;
              }
              lastNumIndex = i;
            }
            

            // Convert the valid subString to Integer number:
            int number = 0; 
            for(int i = firstNumIndex; i<= lastNumIndex; i++)
            {
              number = number * 10 + (int)(buffer[i] - 48);
            }

            // Case the integer is negative:
            if(isNegative)
            {
              number = number * -1;
            }
            machine->WriteRegister(2, number);
            IncreasePC();
            delete buffer;
            return;
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


       IncreasePC();
	     break;
	   }


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
		char *buff = new char[2]; //tao mang de doc chuoi nhap tu phim
		int numbOfChar = gSynchConsole->Read(buff, 2);
		if (numbOfChar == 0)//khong co ky tu nao duoc nhap.	
		{
			DEBUG('a', "Empty character.\n");
			machine->WriteRegister(2, 0);					
		}
		else
			if (numbOfChar == 1)//co 1 ky tu
			{						
				DEBUG('a', "Reading Character ...\n");
				char readChar = buff[0];//luu gia tri vao bien tra ve user mode
				machine->WriteRegister(2, readChar);						
			}
			else//neu co 2 ky tu tro len thi chi tra ve ky tu dau tien doc duoc
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
		printChar = (char)machine->ReadRegister(4);//lay gia tri doc duoc tu tham so.
		gSynchConsole->Write(&printChar, 1);
		IncreasePC();				
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
