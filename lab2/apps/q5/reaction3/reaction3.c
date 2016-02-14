#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define SO2_NUM 1 
#define O_NUM 1 
#define H_NUM 1

void main(int argc, char * argv[])
{
	//Local Variables
	int num = 0, i, j;
	sem_t sem_sulphur, sem_oxygen, sem_hydrogen, sem_acid, proc_sem;

	if(argc != 7)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	num = dstrtol(argv[1], NULL, 10);
	sem_hydrogen = dstrtol(argv[2], NULL, 10);
	sem_oxygen = dstrtol(argv[3], NULL, 10);
	sem_sulphur = dstrtol(argv[4], NULL, 10);
	sem_acid = dstrtol(argv[5], NULL, 10);
  proc_sem = dstrtol(argv[6], NULL, 10);

	for(i = 0; i < num; i++)
	{
		if (sem_wait(sem_hydrogen) != SYNC_SUCCESS) {
   			Printf("Bad semaphore of hydrogen sem wait (%d) in reaction 3", sem_hydrogen); Printf(argv[0]); Printf("\n");
    		Exit();
  		}

		if (sem_wait(sem_oxygen) != SYNC_SUCCESS) {
   			Printf("Bad semaphore of oxygen sem wait (%d) in reaction 3", sem_oxygen); Printf(argv[0]); Printf("\n");
    		Exit();
  		}

		if (sem_wait(sem_sulphur) != SYNC_SUCCESS) {
   			Printf("Bad semaphore of sulphate sem wait (%d) in reaction 3", sem_sulphur); Printf(argv[0]); Printf("\n");
    		Exit();
  		}

		if(sem_signal(sem_acid) != SYNC_SUCCESS) {
 			Printf("Bad semaphore for SO2 increment in reaction 3 : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
  		Exit();
  		}

  		Printf("Created new sulphuric acid (H2S04) molecule\n");
 	}

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in reaction 3 : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}
}