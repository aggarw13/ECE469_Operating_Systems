#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define HYDRO_NUM 2 
#define H_NUM 2 
#define O_NUM 1 


void main(int argc, char * argv[])
{
	//Local Variables
	int num = 0, i, j;
	sem_t sem_hydro, sem_oxygen, sem_hydrogen, proc_sem;


	if(argc != 6)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	num = dstrtol(argv[1], NULL, 10);
	sem_hydro = dstrtol(argv[2], NULL, 10);
	sem_hydrogen = dstrtol(argv[3], NULL, 10);
	sem_oxygen = dstrtol(argv[4], NULL, 10);
	proc_sem = dstrtol(argv[5], NULL, 10);

	for(i = 0; i < num; i++)
	{
		if (sem_wait(sem_hydro) != SYNC_SUCCESS) {
   			Printf("Bad semaphore of hydro sem wait (%d) in reaction 1", sem_hydro); Printf(argv[0]); Printf("\n");
    		Exit();
  		}

  		if (sem_wait(sem_hydro) != SYNC_SUCCESS) {
   			Printf("Bad semaphore of hydro sem wait (%d) in reaction 1", sem_hydro); Printf(argv[0]); Printf("\n");
    		Exit();
  		}

  		for(j = 0; j < H_NUM; j++)
  		{
			if(sem_signal(sem_hydrogen) != SYNC_SUCCESS) {
   			Printf("Bad semaphore for H increment in reaction 1 : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    		Exit();
    		}
    		Printf("Created new hydrogen (H2) molecule\n");
  		}


  		for(j = 0; j < O_NUM; j++)
  		{
			if(sem_signal(sem_oxygen) != SYNC_SUCCESS) {
   			Printf("Bad semaphore for O increment in in reaction 1 : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    		Exit();
  			}
    		Printf("Created new oxygen (O2) molecule\n");
  		}
 	}

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in reaction 1 : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}

}