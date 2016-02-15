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
	char info[12] = "HELLO WORLD";	
	sem_t proc_sem;
	uint32 h_mem;
	int curr_index = 0, process_no;
	int wasEmpty = 0;

	if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <synch lock number> 
    	<cond var 1 number> <cond var 2 number> <handle_to_page_mapped_semaphore>\n"); 
    Exit();
	}

	// Convert the command-line strings into integers for use as handles
	h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  	buff_lock = dstrtol(argv[2], NULL, 10);
  	notEmpty = dstrtol(argv[3], NULL,10);
  	notFull = dstrtol(argv[4], NULL,10);
  	proc_sem = dstrtol(argv[5], NULL, 10);

  	// Map shared memory page into this process's memory space
  	if ((cbuffer = (circ_buffer *)shmat(h_mem)) == NULL) {
    	Printf("Could not map the virtual address to the memory in pid : %d", getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  	}

  	while(curr_index < 11)
  	{
  		// This section attempts to access the circular buffer and place string characters
  		// one by one. 
	  	if(lock_acquire(buff_lock))
	  	{
	  		if(cbuffer->head == (cbuffer->tail + 1) % BUFFERSIZE)
	  		{
	  			Printf("Producer :%d is waiting on notFull\n", getpid());
	  			cond_wait(notFull);
	  		}

	  		if(cbuffer->head == cbuffer->tail)
	  		{
	  			//Printf("Current buffer head : %d and tail : %d", cbuffer->head, cbuffer->tail);
	  			wasEmpty = 1;
	  		}

	  		cbuffer->mem_buffer[cbuffer->tail] = info[curr_index];
	  		Printf("Producer %d inserted: %c\n", getpid(), info[curr_index]);
	  		cbuffer->tail = (cbuffer->tail + 1) % BUFFERSIZE;
	  		curr_index++;
	  	
	  		if(wasEmpty)
	  		{
	  			Printf("Producer %d sends buffer not empty signal\n", getpid());
	  			cond_broadcast(notEmpty);
	  			wasEmpty = 0;
	  		}
	  	}
	  	lock_release(buff_lock);
	}

	/*if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    Printf("Bad semaphore : %d in Producer : (%d) ", proc_sem, getpid()); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
 	}*/

 	Printf("Producer ID : %d has completed its work and will die\n", getpid());


}
