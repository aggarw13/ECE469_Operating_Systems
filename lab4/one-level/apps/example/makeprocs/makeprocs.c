#include "usertraps.h"
#include "misc.h"

#define HELLO_WORLD "hello_world.dlx.obj"
#define INT_MAX 0x7fffffff

void main (int argc, char *argv[])
{
  int num_hello_world = 0;             // Used to store number of processes to create
  int i;                               // Loop index variable
  uint32 * stackgrowth_addr = NULL, * outside_vaddr = NULL;

  sem_t s_procs_completed;             // Semaphore used to wait until all spawned processes have completed
  char s_procs_completed_str[10];      // Used as command-line argument to pass page_mapped handle to new processes

  if (argc != 2) {
    Printf("Usage: %s <number of hello world processes to create>\n", argv[0]);
    Exit();
  }

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
  Printf("makeprocs (%d): Creating 1 hello world process");
  process_create(HELLO_WORLD, s_procs_completed_str, NULL);

  // Accessing Memory Virtual Address outiside the virtual address space
  printf("-------------------------------Part 2--------------------------------\n");
  outside_vaddr = (uint32 *)(0x400003);   //Word address just outisde the virtual memory address
  (*outside_vaddr) = (uint32)INT_MAX;

  // Accessing memory outside the virtual memory access
  Printf("-------------------------------Part 3-------------------------------\n");
  stackgrowth_addr = (uint32 *)(0x3feffc); /Word address just before the user stack page
  (*stackgrowth_addr) = (uint32)(0xff);

  // Creating 100 Hello World processes
  Printf("-------------------------------Part 4-------------------------------\n");
  Printf("Creating a row of 100 Hello World processes in sequence to test correct memory paging allocation and deallocation\n");
  for(i=0; i<num_hello_world; i++) {
    Printf("makeprocs (%d): Creating hello world #%d\n", getpid(), i);
    process_create(HELLO_WORLD, s_procs_completed_str, NULL);
    if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
      Printf("Bad semaphore s_procs_completed (%d) in %s\n", s_procs_completed, argv[0]);
      Exit();
    }
  }

  Printf("-------------------------------------------------------------------------------------\n");
  Printf("makeprocs (%d): All other processes completed, exiting main process.\n", getpid());

}
