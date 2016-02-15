#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define SO2_NUM 1
#define O_NUM 1 


void main(int argc, char * argv[])
{
	//Local Variables
	int num = 0, i, j;
	sem_t sem_sulphate, sem_sulphur, sem_oxygen, proc_sem;

	if(argc != 6)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	num = dstrtol(argv[1], NULL, 10);
	sem_sulphate = dstrtol(argv[2], NULL, 10);
	sem_sulphur = dstrtol(argv[3], NULL, 10);
	sem_oxygen = dstrtol(argv[4], NULL, 10);
	proc_sem = dstrtol(argv[5], NULL, 10);

	for(i = 0; i < num; i++)
	{
		if (sem_wait(sem_sulphate) != SYNC_SUCCESS) {
   			Printf("Bad semaphore of sulphate sem wait (%d) in reaction 2", sem_sulphate); Printf(argv[0]); Printf("\n");
    		Exit();
  		}

  		for(j = 0; j < SO2_NUM; j++)
  		{
			if((sem_signal(sem_sulphur))!= SYNC_SUCCESS) {
   			Printf("Bad semaphore for SO2 increment in reaction 2 : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    		Exit();
    		}
    		Printf("Created new sulphur dioxide (S02) molecule\n");
  		}

  		for(j = 0; j < O_NUM; j++)
  		{
			if(sem_signal(sem_oxygen) != SYNC_SUCCESS) {
   			Printf("Bad semaphore for O increment in reaction 2 : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    		Exit();
  			}
    		Printf("Created new oxygen (O2) molecule\n");
  		}
 	}

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in reaction  : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}
}