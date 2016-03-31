#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"

void main(int argc, char * argv[])
{
	//Local Variables
	int num = 0, i;
	sem_t sem_sulphate, proc_sem;

	if(argc != 4)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	num = dstrtol(argv[1], NULL, 10);
	sem_sulphate = dstrtol(argv[2], NULL, 10);
	proc_sem = dstrtol(argv[3], NULL, 10);

	for(i = 0; i < num; i++)
	{
		if(sem_signal(sem_sulphate) != SYNC_SUCCESS) {
   			Printf("Bad semaphore for sulphate increment in sulphate injection : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    		Exit();
    	}
    	Printf("Created a new sulphate molecule\n");
 	}

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in sulphate injection : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}

}