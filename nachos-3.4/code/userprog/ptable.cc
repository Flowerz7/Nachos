#include "ptable.h"
#include "system.h"
#include "openfile.h"


PTable::PTable(int size)
{
  if (size < 0)
    return;

  psize = size;
  bm = new BitMap(size);
  bmsem = new Semaphore("bmsem",1);

  for(int i = 0; i < MAX_PROCESS; i++){
		pcb[i] = 0;
  }

	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->SetFileName(currentThread->getName());
	pcb[0]->parentID = -1;
}


PTable::~PTable()
{
  if( bm != 0 )
		delete bm;
    
  for(int i = 0; i < psize; i++) {
		if(pcb[i] != 0)
			delete pcb[i];
  }
		
	if( bmsem != 0)
		delete bmsem;
}





int PTable::ExecUpdate(char* name)
{
	// multual exclusive to make sure only one thread is loaded at a time.
	bmsem->P();

	// Check five filenaem is exist?
	OpenFile *executable = fileSystem->Open(name);
	if (executable == NULL) 
	{
		printf("\nUnable to open file %s\n", name);
		bmsem->V();
		return -1;
  }
	delete executable;			

	// Make sure the parent process do not call itsefl.
	if(strcmp(name,currentThread->getName()) == 0 )
	{
		printf("\nPTable::Exec : Can't not execute itself.\n");		
		bmsem->V();
		return -1;
	}

	// Find a free slot in ptable:
	int index = this->GetFreeSlot();
	if(index < 0)
	{
		printf("\nPTable::Exec :There is no free slot.\n");
		bmsem->V();
		return -1;
	}

	// Create new PCB with the proccessID is index:
	pcb[index] = new PCB(index);
	pcb[index]->SetFileName(name);

	// ParentID of new PCB is the thread run this Exec function:
  pcb[index]->parentID = currentThread->processID;

	// Run Exec method of new PCB:
	int pid = pcb[index]->Exec(name,index);

	bmsem->V();

	return pid;
}





int PTable::ExitUpdate(int ec)
{
//Kiem tra pID co ton tai khong
	int pID= currentThread->processID;
	if(!IsExist(pID))
	{
		printf("\nLoi: Tien trinh khong ton tai !!!\n");
		return -1;
	}
//////////////////////////////////////////////////////////////

//Neu la main process thi Halt()
	if(pID==0)
	{
		interrupt->Halt();
		return 0;
	}
/////////////////////////////////////////////////////////////

	pcb[pID]->SetExitCode(ec);
	
	if(pcb[pID]->JoinStatus != -1)
	{
		pcb[pID]->JoinRelease();
		pcb[pID]->ExitWait();
		Remove(pID);	
	}
	else
		Remove(pID);
	return ec;
}


int PTable::JoinUpdate(int pID)
{
	
	if(pID <= 0 || pID > 9)
	{
		printf("\nLoi: Khong ton tai process: id = %d\n",pID);
		return -1;
	}

	if (pcb[pID] == NULL)
	{
		printf("Loi: Khong ton tai process id nay!");
		return -1;
	}

//kiem tra tien trinh dang chay co la cha cua tien trinh can join hay khong
	if(currentThread->processID != pcb[pID]->parentID)
	{
		printf("\nLoi: Ko duoc phep join vao tien trinh khong phai cha cua no !!!\n");
		return -1;
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	

	pcb[pID]->JoinWait(); 	//doi den khi tien trinh con ket thuc

	int ec = pcb[pID]->GetExitCode();

	if(ec != 0)
	{
		printf("\nProcess exit with exitcode EC = %d ",ec);
		return -1;
	}

	pcb[pID]->ExitRelease();	//cho phep tien trinh con ket thuc
	
	return 0;
}


// Find free slot in order to save the new process infom
int PTable::GetFreeSlot()
{
	return bm->Find();
}

// Check if Process ID is Exist
bool PTable::IsExist(int pid)
{
	return bm->Test(pid);
}

// Remove proccess ID out of table
// When it ends
void PTable::Remove(int pid)
{
	bm->Clear(pid);
	if(pcb[pid] != 0)
		delete pcb[pid];
}

char* PTable::GetFileName(int id)
{
	return (pcb[id]->GetFileName());
}
