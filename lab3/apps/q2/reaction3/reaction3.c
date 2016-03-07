#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"
#define SO2_NUM 1 
#define O_NUM 1 
#define H_NUM 1
#define ACID_LEN 6
#define O_LEN 4
#define H_LEN 4
#define SULPHUR_LEN 4

void main(int argc, char * argv[])
{
	//Local Variables
	mbox_t mbox_sulphur, mbox_oxygen, mbox_hydrogen, mbox_acid;
	sem_t proc_sem;
	char hydrogen_mesg[H_LEN], oxygen_mesg[O_LEN], sulphur_mesg[SULPHUR_LEN], acid_mesg[ACID_LEN] = "H2S04";

	if(argc != 6)
	{
		Printf("Usage: "); Printf(argv[0]); 
	    Exit();
	}

	mbox_hydrogen = dstrtol(argv[1], NULL, 10);
	mbox_oxygen = dstrtol(argv[2], NULL, 10);
	mbox_sulphur = dstrtol(argv[3], NULL, 10);
	mbox_acid = dstrtol(argv[4], NULL, 10);
  proc_sem = dstrtol(argv[5], NULL, 10);

  	// Opening mailboxes of reactions molecules
  	if (mbox_open(mbox_hydrogen) == MBOX_FAIL) {
      Printf("reaction 3 (%d): Could not open the hyrogen mailbox!\n", getpid());
      Exit();
    }

    if (mbox_open(mbox_oxygen) == MBOX_FAIL) {
      Printf("reaction 3 (%d): Could not open the oxygen mailbox!\n", getpid());
      Exit();
    }

    if (mbox_open(mbox_sulphur) == MBOX_FAIL) {
      Printf("reaction 3 (%d): Could not open the sulphur mailbox!\n", getpid());
      Exit();
    }

  	if (mbox_open(mbox_acid) == MBOX_FAIL) {
      Printf("reaction 3 (%d): Could not open the H2S04 mailbox!\n", getpid());
      Exit();
    }

    // Consume reactant molecules

  	if (mbox_recv(mbox_oxygen, O_LEN, (void *)&oxygen_mesg) == MBOX_FAIL) {
      Printf("Bad oxygen mailbox : %d message receive in reaction 3 from MB : !\n", mbox_oxygen);
      Exit();
    }

	if (mbox_recv(mbox_hydrogen, H_LEN, (void *)&hydrogen_mesg) == MBOX_FAIL) {
   		Printf("Bad hydrogen mailbox :%d message receive in reaction 3", mbox_hydrogen); Printf(argv[0]); Printf("\n");
    	Exit();
  	}

	if (mbox_recv(mbox_sulphur, SULPHUR_LEN, (void *)&sulphur_mesg) == MBOX_FAIL) {
   		Printf("Bad sulphur mailbox :%d message receive in reaction 3", mbox_sulphur); Printf(argv[0]); Printf("\n");
    	Exit();
  	}

  	if(mbox_send(mbox_acid, ACID_LEN, (void *)&acid_mesg) == MBOX_FAIL) {
      Printf("Bad message sending to H2SO4 mbox : (%d) in reaction 3", mbox_acid); Printf(argv[0]); Printf("\n");
      Exit();
	}

  Printf("Created new sulphuric acid (H2S04) molecule\n");

  if (mbox_close(mbox_hydrogen) == MBOX_FAIL) {
      Printf("reaction 3 (%d): Could not close the hyrogen mailbox!\n", getpid());
      Exit();
  }

  if (mbox_close(mbox_oxygen) == MBOX_FAIL) {
    Printf("reaction 3 (%d): Could not close the oxygen mailbox!\n", getpid());
    Exit();
  }

  if (mbox_close(mbox_sulphur) == MBOX_FAIL) {
    Printf("reaction 3 (%d): Could not close the sulphur mailbox!\n", getpid());
    Exit();
  }

  if (mbox_close(mbox_acid) == MBOX_FAIL) {
    Printf("reaction 3 (%d): Could not close the H2S04 mailbox!\n", getpid());
    Exit();
  }


 	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    	Printf("Bad semaphore for proc sem increment in reaction 3 : %d ", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
	}
}