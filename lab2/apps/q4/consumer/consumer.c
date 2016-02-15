#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "circular_buffer.h"

void main(int argc, char * argv[])
{
	//Local variables
	lock_t buff_lock;
	cond_t notEmpty, notFull;
	circ_buffer * cbuffer;	
	sem_t proc_sem;
	uint32 h_mem;
	int curr_index = 0, process_no;
	int wasFull = 0;

	if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <synch lock number>
    <cond var 1 number> <cond var 2 number> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
	}

	// Convert the command-line strings into integers for use as handles
	h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
	buff_lock = dstrtol(argv[2], NULL, 10);
	notEmpty = dstrtol(argv[3], NULL, 10);
	notFull = dstrtol(argv[4], NULL, 10);
	proc_sem = dstrtol(argv[5], NULL, 10);

	// Map shared memory page into this process's memory space
	if ((cbuffer = (circ_buffer *)shmat(h_mem)) == NULL) {
  	Printf("Could not map the virtual address to the memory in pid : %d\n", getpid()); Printf(argv[0]); Printf(", exiting...\n");
  Exit();
	}

	while(curr_index < 11)
	{
		// This section attempts to access the circular buffer, wait it buffer is full and consume characters
		// one by one by one.  It  notifiers waiting producer processses about availibility of buffer space
  	if(lock_acquire(buff_lock))
  	{
  		while(cbuffer->tail == cbuffer->head)
  		{
  			if(cond_wait(notEmpty) == SYNC_FAIL)
        {
           Printf("Bad cond wait in consumer "); Printf(argv[0]); 
           Exit(); 
        }
      }

  		if(cbuffer->head == (cbuffer->tail + 1) % BUFFERSIZE)
				wasFull = 1;

			Printf("Consumer %d removed: %c\n", getpid(), cbuffer->mem_buffer[cbuffer->head]);
			curr_index++;
			cbuffer->head = (cbuffer->head + 1) % BUFFERSIZE;

			if(wasFull)
			{	
				//Printf("Conusmer %d sends signal of buffer non-stauration (nonFull)\n", getpid());
				if(cond_signal(notFull) == SYNC_FAIL)
        {
          Printf("Bad cond broadcast in consumer"); Printf(argv[0]); 
          Exit();
        }
				wasFull = 0;
			}
  	}
		lock_release(buff_lock);
	}

	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    Printf("Bad semaphore : %d in Consumer(%d) in ", proc_sem, getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}

 	//Printf("Consumer ID : %d has completed its work and will die\n", getpid());


}
