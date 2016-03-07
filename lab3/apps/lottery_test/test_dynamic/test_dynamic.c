#include "lab2-api.h"

/*
  This is the testcase for question 3
  Note: Dynamic priority should be disabled
  Expected output is described below.
*/

main (int argc, char *argv[])
{
  int number, i, j, offset;
  sem_t sem_dyn;

  sem_dyn = dstrtol(argv[1], NULL, 10);
  while(1)
  {	
    //Printf("Testing Dynamic Lottery Scheduling\n");
    for(i = 0; i < 10000; i++)
    {
  	 if (i % 1000 == 0)
  	 	sem_wait(sem_dyn);
    }
    //Printf("Completed wait by test proc\n");
  }
 
  Printf("Ending process : %d for Dynamic test\n", getpid());
}


