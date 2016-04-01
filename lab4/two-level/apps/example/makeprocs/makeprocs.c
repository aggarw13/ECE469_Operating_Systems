#include "usertraps.h"
#include "misc.h"

#define HELLO_WORLD "hello_world.dlx.obj"
#define SPAWN_COUNTING "spawn_counting.dlx.obj"
#define INT_MAX 0x7fffffff
#define NUM_SPAWN_PROCS 30

void consume_userstack()
{
  char large_buff[4096];
  int new_page_var;
  Printf("After allocating 4KB character buffer, new variable has address : 0x%x\n", (unsigned int)&new_page_var);
}

void main (int argc, char *argv[])
{
  int num_hello_world = 0;             // Used to store number of processes to create
  int i;                               // Loop index variable
  int * stackgrowth_addr = NULL, * outside_vaddr = NULL;
  int menu_choice;
  sem_t s_procs_completed;             // Semaphore used to wait until all spawned processes have completed
  char s_procs_completed_str[10];      // Used as command-line argument to pass page_mapped handle to new processes

  if (argc != 2) {
    Printf("Usage: %s <number of hello world processes to create>\n", argv[0]);
    Exit();
  }

  Printf("------------------------User Process Execution Menu (Enter choice number)--------------------\n");
  Printf("1.Run Hello World process\n");
  Printf("2.Access memory outside allocated process pages\n");
  Printf("3.Cause user stack to grow more than 1 page\n");
  Printf("4.Call Hello World program 100 in a row\n");
  Printf("5.Spwan 330 processes simultaenously to count to a large value\n");
  Printf("6.Access memory beyond the virtual memory address space\n");
  
  // Convert string from ascii command line argument to integer number
  num_hello_world = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  Printf("makeprocs (%d): Creating %d hello_world processes\n", getpid(), num_hello_world);

  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.
  if ((s_procs_completed = sem_create(0)) == SYNC_FAIL) {
    Printf("makeprocs (%d): Bad sem_create\n", getpid());
    Exit();
  }

  // Setup the command-line arguments for the new processes.  We're going to
  // pass the handles to the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  ditoa(s_procs_completed, s_procs_completed_str);
  
  // Create 1 Hello World process
  Printf("-------------------------------Part 1------------------------------\n");
  Printf("makeprocs (%d): Creating 1 hello world process\n");
  process_create(HELLO_WORLD, s_procs_completed_str, NULL);

  sem_wait(s_procs_completed);

  // Accessing Memory Virtual Address outiside the virtual address space
  /*Printf("-------------------------------Part 2--------------------------------\n");
  outside_vaddr = (int *)(0x400003);   //Word address just outisde the virtual memory address
  (*outside_vaddr) = (unsigned int)INT_MAX;*/

  // Make user stack grow beyond a page  
  Printf("-------------------------------Part 3-------------------------------\n");
  //consume_userstack();
  //new_page_var = (unsigned int)large_buff;
  
  // Creating 100 Hello World processes
  /*Printf("-------------------------------Part 4-------------------------------\n");
  Printf("Creating a row of 100 Hello World processes in sequence to test correct memory paging allocation and deallocation\n");
  for(i=0; i<100; i++) {
    Printf("makeprocs (%d): Creating hello world #%d\n", getpid(), i);
    process_create(HELLO_WORLD, s_procs_completed_str, NULL);
    if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
      Exit();
    }
  //sem_wait(s_procs_completed);
  }
  Printf("Completed execution of 100 sequential Hello World processes with successful memory management\n");
  */
  // Creating 30 simultaneous large counting processes for memory exhaustion test
 /* Printf("-----------------------------Part 5----------------------------------\n");
  if ((s_procs_completed = sem_create(-NUM_SPAWN_PROCS + 1)) == SYNC_FAIL) {
    Printf("makeprocs (%d): Bad sem_create\n", getpid());
    Exit();
  }

  // Setup the command-line arguments for the new processes.  We're going to
  // pass the handles to the semaphore as strings
  // on the command line, so we must first convert them from ints to strings.
  ditoa(s_procs_completed, s_procs_completed_str);
  for(i=0; i < 30; i++)
  {
    Printf("makeprocs (%d): Creating counting process, spawn_counting #%d\n", getpid(), i);
    process_create(SPAWN_COUNTING, s_procs_completed_str, NULL);
  }

  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
      Exit();
 }*/
 
  // Accessing memory outside the virtual memory access
  /*Printf("-------------------------------Part 6-------------------------------\n");
  stackgrowth_addr = (unsigned int *)(0x3feffc); //Word address just before the user stack page
  (*stackgrowth_addr) = (unsigned int)(0xff);
*/

  Printf("-------------------------------------------------------------------------------------\n");
  Printf("makeprocs (%d): All other processes completed, exiting main process.\n", getpid());

}
