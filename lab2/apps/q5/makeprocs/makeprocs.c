#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "chemical_reactions.h"

void main (int argc, char *argv[])
{
  int num_hydro = 0, num_sulphate = 0;               // Used to store number of processes to create
  int consum_react1, consum_react2, consum_react3;
  char react1_str[10], react2_str[10], react3_str[10], num_hydro_str[10];
  int i;                          // Loop index variable
  sem_t process_completed;        // Semaphore used to wait until all spawned processes have completed
  sem_t sem_oxygen, sem_hydrogen, sem_hydro, sem_sulphur, sem_sulphate, sem_acid;
  char oxygen_str[10], hydrogen_str[10], hydro_str[10], sulphur_str[10], sulphate_str[10], acid_str[10];
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes

  if (argc != 3) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of H20 processes to create> <number of S04 molecules to create>\n");
    Exit();
  }

  // Convert string from ascii command line argument to integer number
  num_hydro = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  num_sulphate = dstrtol(argv[2], NULL, 10);
  Printf("Creating %d injection and reaction processes\n", num_hydro + num_sulphate);
  
  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.  To do this, we will initialize
  // the semaphore to (-1) * (number of signals), where "number of signals"
  // should be equal to the number of processes we're spawning - 1.  Once 
  // each of the processes has signaled, the semaphore should be back to
  // zero and the final sem_wait below will return.
  if ((process_completed = sem_create(-4)) == SYNC_FAIL) {
    Printf("Bad sem_create for process_completion "); Printf(argv[0]); 
    Exit();
  }

  if ((sem_oxygen = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create for oxygen "); Printf(argv[0]); 
    Exit();
  }

  if ((sem_hydro = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create for hydro "); Printf(argv[0]); 
    Exit();
  }

  if ((sem_sulphate = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create for sulphate "); Printf(argv[0]); 
    Exit();
  }

  if ((sem_sulphur = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create for sulphur "); Printf(argv[0]); 
    Exit();
  }

  if ((sem_hydrogen = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create for hydrogen "); Printf(argv[0]); 
    Exit();
  }

  if ((sem_acid = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create for acid "); Printf(argv[0]); 
    Exit();
  }
  // Calculating expected molecular consumption in each reaction
  consum_react1 = num_hydro / 2;
  consum_react2 = num_sulphate;
  consum_react3 = ((num_hydro / 2) * 2 < num_sulphate)? (num_hydro / 2) * 2 : num_sulphate;

  // Setup the command-line arguments for the new process.  We're going to
  // pass the handles to the shared memory page and the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  //ditoa((num_hydro % 2 == 0)? num_hydro / 2 : num_hydro / 2 + 1, num_hydro_str);
  ditoa(consum_react1, react1_str);
  ditoa(consum_react2, react2_str);
  ditoa(consum_react3, react3_str);
  ditoa(sem_oxygen, oxygen_str);
  ditoa(sem_hydrogen, hydrogen_str);
  ditoa(sem_hydro, hydro_str);
  ditoa(sem_sulphate, sulphate_str);
  ditoa(sem_sulphur, sulphur_str);
  ditoa(sem_acid, acid_str);
  ditoa(process_completed, s_procs_completed_str);   

  // Creating injection and reaction processes
  process_create(INJECTION_1, argv[1], hydro_str, s_procs_completed_str, NULL); 
  process_create(INJECTION_2, argv[2], sulphate_str, s_procs_completed_str, NULL); 
  process_create(REACTION_1, react1_str, hydro_str, hydrogen_str, oxygen_str, s_procs_completed_str, NULL); 
  process_create(REACTION_2, react2_str, sulphate_str, sulphur_str, oxygen_str, s_procs_completed_str, NULL); 
  process_create(REACTION_3, react3_str, hydrogen_str, oxygen_str, sulphur_str, acid_str, s_procs_completed_str, NULL); 

  // And finally, wait until all spawned processes have finished.
  if (sem_wait(process_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", process_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");
}
