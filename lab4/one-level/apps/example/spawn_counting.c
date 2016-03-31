#define MAX_COUNT 0x3fffff

void main(int argc, char ** argv)
{
	int count; //Counter variable
	sem_t proc_sem; //Semaphore handle

	if(argc != 2)
	{
		Printf("Usage: %s <semaphore handle>\n",argv[0]);
		Exit();
	}

	proc_sem = dstrtol(argv[1], NULL, 10);

	Printf("Child Process : %d is beginning to count\n", getpid());

	for(count = 0; count < MAX_COUNT; count++)
	{
		if(count == MAX_COUNT / 2)
			Printf("Child Process : %d has completed half of the counting sequence\n", getpid())
	}

	//Signal parent process about process acompletion
	if(sem_signal(proc_sem) != SYNC_SUCCESS) {
    Printf("spawn_counting (%d): Bad semaphore proc_sem (%d)!\n", getpid(), proc_sem);
    Exit();
  }

  Printf("Spawn Counting :%d has completed counting!\n", getpid());
}


