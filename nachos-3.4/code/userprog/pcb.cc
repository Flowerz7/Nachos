#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

// Extern MyStartProcess() method from progtess.cc
extern void MyStartProcess(int id);

// Contructor:
PCB::PCB(int id)
{
	// Set parentID:
  if (id == 0)
      this->parentID = -1;
  else
      this->parentID = currentThread->processID;

	// Set numwait, exitcode, boolBG and thread pointer:
	this->numwait = this->exitcode = this->boolBG = 0;
	this->thread = NULL;

	// Create joinsem, exitsem and multex semaphore object:
	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
}

// Destructor:
PCB::~PCB()
{
	
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	if(thread != NULL)
	{		
		thread->FreeSpace();
		thread->Finish();
	}
}


int PCB::GetID(){ return this->thread->processID; }
int PCB::GetNumWait() { return this->numwait; }
int PCB::GetExitCode() { return this->exitcode; }
void PCB::SetExitCode(int ec){ this->exitcode = ec; }

void PCB::JoinWait()
{
	// Descrese semaphore count of joinsem:
  joinsem->P();
}

void PCB::JoinRelease()
{ 
	// Increase semaphore count of joinsem:
  joinsem->V();
}

void PCB::ExitWait()
{ 
	// Descrease semaphore count of exitsem:
  exitsem->P();
}

void PCB::ExitRelease() 
{
	// Increase semaphore count of exitsem:
  exitsem->V();
}

void PCB::IncNumWait()
{
	multex->P();
	++numwait;
	multex->V();
}

void PCB::DecNumWait()
{
	multex->P();
	if(numwait > 0)
		--numwait;
	multex->V();
}

void PCB::SetFileName(char* fn){ strcpy(FileName,fn);}
char* PCB::GetFileName() { return this->FileName; }


// Execute the thread:
int PCB::Exec(char* filename, int id)
{  
	// Using multual exclusive semaphore to make sure only one thread is forked at
	// a time:
	multex->P();

	// Create thread that run the program in given filaname:
	this->thread = new Thread(filename);
	if(this->thread == NULL){
		printf("\nPCB::Exec:: Not enough memory..!\n");
    multex->V();
		return -1;
	}

	this->thread->processID = id;
	// parentID of this thread is the thread which call Exec:
	this->parentID = currentThread->processID;

	// Fork this thread with given MyStartProcess in progtest.cc.
 	this->thread->Fork(MyStartProcess,id);

  multex->V();
	return id;
}
