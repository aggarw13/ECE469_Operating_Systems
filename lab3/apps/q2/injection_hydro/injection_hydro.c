#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define MESG_LEN 4

void main(int argc, char * argv[])
{
	//Local Variables
	int i;
	mbox_t mbox_hydro;
	sem_t proc_sem;
	char molecule[MESG_LEN] = "H20";

	if(argc != 3)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	mbox_hydro = dstrtol(argv[1], NULL, 10);
	proc_sem = dstrtol(argv[2], NULL, 10);

	//Open the water molecule mailbox
	if (mbox_open(mbox_hydro) == MBOX_FAIL) {
    	Printf("injection_hydro (%d): Could not open the mailbox!\n", getpid());
    	Exit();
  	}

	if(mbox_send(mbox_hydro, MESG_LEN, (void *)&molecule) != SYNC_SUCCESS) {
   		Printf("Bad mbox message send in hydro injection : (%d) ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    	Exit();
    }

    Printf("Created a new H2O molecule\n");    	

	if (mbox_close(mbox_hydro) == MBOX_FAIL) {
    	Printf("injection_hydro (%d): Could not close the mailbox!\n", getpid());
    	Exit();
  	}

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in hydro injection : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}

}