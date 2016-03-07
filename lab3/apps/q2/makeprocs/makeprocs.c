#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"

void main (int argc, char *argv[])
{
  int num_hydro = 0, num_sulphate = 0;               // Used to store number of processes to create
  int consum_react1, consum_react2, consum_react3;
  //char react1_str[10], react2_str[10], react3_str[10], num_hydro_str[10];
  int max_procs = 0, i, total_childprocs = 0;                          // Loop index variable
  sem_t process_completed;        // Semaphore used to wait until all spawned processes have completed
  mbox_t mbox_oxygen, mbox_hydrogen, mbox_hydro, mbox_sulphur, mbox_sulphate, mbox_acid;
  char oxygen_str[10], hydrogen_str[10], hydro_str[10], sulphur_str[10], sulphate_str[10], acid_str[10];
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes

  if (argc != 3) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of H20 processes to create> <number of S04 molecules to create>\n");
    Exit();
  }

  // Convert string from ascii command line argument to integer number
  num_hydro = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  num_sulphate = dstrtol(argv[2], NULL, 10);
  //Printf("Creating %d injection and reaction processes\n", num_hydro + num_sulphate);
  
  // Calculating expected molecular consumption in each reaction
  consum_react1 = num_hydro / 2;
  consum_react2 = num_sulphate;
  consum_react3 = ((num_hydro / 2) * 2 < num_sulphate)? (num_hydro / 2) * 2 : num_sulphate;
  total_childprocs = consum_react1 + consum_react2 + consum_react3 + num_hydro + num_sulphate; 

  max_procs = (num_sulphate > (num_hydro / 2) * 2)? num_sulphate : (num_hydro / 2) * 2;

  // Creating mailboxes for each molecule by calling usertrap mmbox_create()
  if ((process_completed = sem_create(-total_childprocs + 1)) == SYNC_FAIL) {
    Printf("Bad sem_create for process_completion "); Printf(argv[0]); 
    Exit();
  }

  if ((mbox_oxygen = mbox_create()) == SYNC_FAIL) {
    Printf("Bad mbox_create for oxygen "); Printf(argv[0]); 
    Exit();
  }

  if ((mbox_hydro = mbox_create()) == SYNC_FAIL) {
    Printf("Bad mbox_create for hydro "); Printf(argv[0]); 
    Exit();
  }

  if ((mbox_sulphate = mbox_create()) == SYNC_FAIL) {
    Printf("Bad mbox_create for sulphate "); Printf(argv[0]); 
    Exit();
  }

  if ((mbox_sulphur = mbox_create()) == SYNC_FAIL) {
    Printf("Bad mbox_create for sulphur "); Printf(argv[0]); 
    Exit();
  }

  if ((mbox_hydrogen = mbox_create()) == SYNC_FAIL) {
    Printf("Bad mbox_create for hydrogen "); Printf(argv[0]); 
    Exit();
  }

  if ((mbox_acid = mbox_create()) == SYNC_FAIL) {
    Printf("Bad mbox_create for acid "); Printf(argv[0]); 
    Exit();
  }

  // Opening mailboxes in parent process to ensure liveliness of mailboxes 
  // even after the death of all child processes
  if (mbox_open(mbox_oxygen) == SYNC_FAIL) {
    Printf("Could not open mailbox for oxygen "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_open(mbox_hydro) == SYNC_FAIL) {
    Printf("Could not open mailbox for hydro "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_open(mbox_sulphate ) == SYNC_FAIL) {
    Printf("Could not open mailbox for sulphate "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_open(mbox_sulphur) == SYNC_FAIL) {
    Printf("Could not open mailbox for sulphur "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_open(mbox_hydrogen) == SYNC_FAIL) {
    Printf("Could not open mailbox for hydrogen "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_open(mbox_acid) == SYNC_FAIL) {
    Printf("Could not open mailbox for acid "); Printf(argv[0]); 
    Exit();
  }

  // Setup the command-line arguments for the new processes.  We're going to
  // pass the handles to the mailboxes and the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  //ditoa(consum_react1, react1_str);
  //ditoa(consum_react2, react2_str);
  //ditoa(consum_react3, react3_str);
  ditoa(mbox_oxygen, oxygen_str);
  ditoa(mbox_hydrogen, hydrogen_str);
  ditoa(mbox_hydro, hydro_str);
  ditoa(mbox_sulphate, sulphate_str);
  ditoa(mbox_sulphur, sulphur_str);
  ditoa(mbox_acid, acid_str);
  ditoa(process_completed, s_procs_completed_str);   

  for(i = 0; i < max_procs; i++)
  {
    //Printf("Creating injection and reaction processes for iteration : %d\n", i); 
    if(i < num_hydro)
      process_create(INJECTION_1, 0, 0, hydro_str, s_procs_completed_str, NULL); 

    //Printf("Reached the point of injection 2 creation\n");
    
    if(i < num_sulphate)
      process_create(INJECTION_2, 0, 0, sulphate_str, s_procs_completed_str, NULL); 
   
      //Printf("Reached the point of reaction 1 process\n");  
    if(i < consum_react1)
      process_create(REACTION_1, 0, 0, hydro_str, hydrogen_str, oxygen_str, s_procs_completed_str, NULL); 
  
    if(i < consum_react2)
      process_create(REACTION_2, 0, 0, sulphate_str, sulphur_str, oxygen_str, s_procs_completed_str, NULL); 
    
    if(i < consum_react3)
      process_create(REACTION_3, 0, 0, hydrogen_str, oxygen_str, sulphur_str, acid_str, s_procs_completed_str, NULL); 
  
    //Printf("Created reaction 3 process\n");  
  }
   
  // And finally, wait until all spawned processes have finished.
  if (sem_wait(process_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", process_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if (mbox_close(mbox_oxygen) == SYNC_FAIL) {
    Printf("Could not close mailbox for oxygen "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_close(mbox_hydro) == SYNC_FAIL) {
    Printf("Could not close mailbox for hydro "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_close(mbox_sulphate ) == SYNC_FAIL) {
    Printf("Could not close mailbox for sulphate "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_close(mbox_sulphur) == SYNC_FAIL) {
    Printf("Could not close mailbox for sulphur "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_close(mbox_hydrogen) == SYNC_FAIL) {
    Printf("Could not close mailbox for hydrogen "); Printf(argv[0]); 
    Exit();
  }

  if (mbox_close(mbox_acid) == SYNC_FAIL) {
    Printf("Could not close mailbox for acid "); Printf(argv[0]); 
    Exit();
  }

  Printf("All other processes completed, exiting main process.\n");
}