#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define HYDRO_NUM 2 
#define H_NUM 2 
#define O_NUM 1 
#define HYDRO_LEN 4
#define H_LEN 3
#define O_LEN 3

void main(int argc, char * argv[])
{
	//Local Variables
	int i, j;
	mbox_t mbox_hydro, mbox_oxygen, mbox_hydrogen;
  sem_t proc_sem;
  char hydro[HYDRO_LEN], hydrogen_mesg[H_LEN] = "H2", oxygen_mesg[O_LEN] = "O2";

	if(argc != 7)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	mbox_hydro = dstrtol(argv[3], NULL, 10);
	mbox_hydrogen = dstrtol(argv[4], NULL, 10);
	mbox_oxygen = dstrtol(argv[5], NULL, 10);
	proc_sem = dstrtol(argv[6], NULL, 10);

  //Open reaction molecule mailboxes
  if (mbox_open(mbox_hydro) == MBOX_FAIL) {
      Printf("reaction 1 (%d): Could not open the water mailbox!\n", getpid());
      Exit();
    }

  if (mbox_open(mbox_hydrogen) == MBOX_FAIL) {
      Printf("reaction 1 (%d): Could not open the hydrogen mailbox!\n", getpid());
      Exit();
    }

  if (mbox_open(mbox_oxygen) == MBOX_FAIL) {
      Printf("reaction 1 (%d): Could not open the oxygen mailbox!\n", getpid());
      Exit();
    }

  // Receive hwater molecules
  for(i = 0; i < HYDRO_NUM; i++)
  {
  	if (mbox_recv(mbox_hydro, HYDRO_LEN, (void *)&hydro) != SYNC_SUCCESS) {
    		Printf("Bad water mailbox message receive in reaction 1", mbox_hydro); Printf(argv[0]); Printf("\n");
     		Exit();
    }
  }

  // Generate hydrogen molecules
	for(j = 0; j < H_NUM; j++)
	{
    if (mbox_send(mbox_hydrogen, H_LEN, (void *)&hydrogen_mesg) != SYNC_SUCCESS) {
      Printf("Bad mailbox message sending of hydrogen mbox : (%d) in reaction 1", mbox_hydro); Printf(argv[0]); Printf("\n");
      Exit();
    }

		Printf("Created new hydrogen (H2) molecule\n");
	}

  // Generate oxygen molecules
  for(j = 0; j < O_NUM; j++)
	{
	  if(mbox_send(mbox_oxygen, O_LEN, (void *)&oxygen_mesg) != SYNC_SUCCESS) {
		  Printf("Bad mailbox message sending for oxygen mbox : %d in reaction 1 : (%d)\n", getpid()); Printf(argv[0]); Printf(", exiting...\n");
		  Exit();
		}

		Printf("Created new oxygen (O2) molecule\n");
	}

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in reaction 1 : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}

}