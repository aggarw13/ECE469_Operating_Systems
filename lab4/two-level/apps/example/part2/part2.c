#include "usertraps.h"
#include "misc.h"

void main (int argc, char *argv[]) {
  sem_t sem;
  sem = dstrtol(argv[1], NULL, 10);

  Printf("Accessing virtual memory address before the current user stack page in child process : %d!\n", getpid());  

  sem_signal(sem);
  Printf("%d\n", *(int*)0x3feffc);
}
