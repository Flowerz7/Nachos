#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAX_PROCESS 10

class PTable
{
private:
    // PTable size.
	  int psize;

    // Management memmory by BitMap class.
	  BitMap *bm;                 

    // PCB objects array.
	  PCB* pcb[MAX_PROCESS];

    // Multual exclusive semaphore for PTable.
	  Semaphore* bmsem;           

public:
    // Constructor:
     PTable(int = 10);         
                              
    // Destructor:
    ~PTable();                  
		
    // Calling in Exec syscall:
    int ExecUpdate(char*);      

    int ExitUpdate(int);        

    int JoinUpdate(int);        

    int GetFreeSlot();          

    bool IsExist(int pid);      
    
    // Remove a process from ptable after it done.
    void Remove(int pid);       

    char* GetFileName(int id);  
};
#endif 
