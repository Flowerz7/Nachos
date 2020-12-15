#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

// Process Control Block Class
class PCB
{
private:
    // JoinProcess's semaphore.
    Semaphore* joinsem;         

    // ExitProcess's semaphore. 
    Semaphore* exitsem;         

    // Multual Exclusive semaphore.
    Semaphore* multex;          

    // Exitcode.
    int exitcode;		

    // Number of process which joined.
    int numwait;                

    // Name of process.
    char FileName[32];          

    // The thread in this PCB. 
    Thread* thread;             
public:
    // ID of parent process.
    int parentID;          
    int JoinStatus;
    
    // Constructor:
    PCB(int = 0);               

    // Desctrutor:
    ~PCB();                     

    // Create a new thread:
    int Exec(char*,int);      

    // Return ProcessID:
    int GetID();             

    // Return NumWait:
    int GetNumWait();       

    void JoinWait();            
                        
    void ExitWait();             

    void JoinRelease();         

    void ExitRelease();        

    // Increasing NumWait:
    void IncNumWait();     

    // Descreasing NumWait:
    void DecNumWait();      

    // Set ExitCode:
    void SetExitCode(int);   

    // Get ExitCode:
    int GetExitCode();        

    // Set name of process:
    void SetFileName(char*);   

    // Get name of process:
    char* GetFileName();        
};

#endif // PCB_H
