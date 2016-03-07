#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define SO2_NUM 1
#define O_NUM 1 
#define SULPHATE_LEN 6
#define SO2_LEN 4
#define O_LEN 3

void main(int argc, char * argv[])
{
	//Local Variables
	int num = 0, i, j;
	mbox_t mbox_sulphate, mbox_sulphur, mbox_oxygen;
  sem_t proc_sem;
  char sulphate[SULPHATE_LEN], sulphur_mesg[SO2_LEN] = "SO2", oxygen_mesg[O_LEN] = "O2";

	if(argc != 5)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	mbox_sulphate = dstrtol(argv[1], NULL, 10);
	mbox_sulphur = dstrtol(argv[2], NULL, 10);
	mbox_oxygen = dstrtol(argv[3], NULL, 10);
	proc_sem = dstrtol(argv[4], NULL, 10);

	//Open reaction molecule mailboxes
  if (mbox_open(mbox_sulphate) == MBOX_FAIL) {
      Printf("rection 2(%d): Could not open the sulphate mailbox!\n", getpid());
      Exit();
  }

   if (mbox_open(mbox_sulphur) == MBOX_FAIL) {
      Printf("reaction 2 (%d): Could not open the sulphur mailbox!\n", getpid());
      Exit();
    }

  if (mbox_open(mbox_oxygen) == MBOX_FAIL) {
      Printf("reaction 2 (%d): Could not open the oxygen mailbox!\n", getpid());
      Exit();
    }

  if (mbox_recv(mbox_sulphate, SULPHATE_LEN, sulphate) == 0) {
        Printf("Bad sulphate mailbox : %d message receive in reaction 2", mbox_sulphate); Printf(argv[0]); Printf("\n");
        Exit();
  }

  for(j = 0; j < SO2_NUM; j++)
	{
	  if (mbox_send(mbox_sulphur, SO2_LEN, (void *)&sulphur_mesg) != SYNC_SUCCESS) {
      Printf("Bad message sending to sulphur mbox : (%d) in reaction 2", mbox_sulphur); Printf(argv[0]); Printf("\n");
      Exit();
    }
  	Printf("Created new sulphur dioxide (S02) molecule\n");
	}

	for(j = 0; j < O_NUM; j++)
	{
		if(mbox_send(mbox_oxygen, O_LEN, (void *)&oxygen_mesg) != SYNC_SUCCESS) {
      Printf("Bad message sending to oxygen mbox : (%d) in reaction 2", mbox_oxygen); Printf(argv[0]); Printf("\n");
      Exit();
		}
  		Printf("Created new oxygen (O2) molecule\n");
	}

  if (mbox_close(mbox_sulphate) == MBOX_FAIL) {
      Printf("rection 2(%d): Could not close the sulphate mailbox!\n", getpid());
      Exit();
  }

   if (mbox_close(mbox_sulphur) == MBOX_FAIL) {
      Printf("reaction 2 (%d): Could not close the sulphur mailbox!\n", getpid());
      Exit();
    }

  if (mbox_close(mbox_oxygen) == MBOX_FAIL) {
      Printf("reaction 2 (%d): Could not close the oxygen mailbox!\n", getpid());
      Exit();
    }

 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in reaction 2  : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}
}