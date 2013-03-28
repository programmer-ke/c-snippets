#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

volatile struct shared { double pi; int lock; } *shared;

inline extern int xchg(register int reg,
		       volatile int * volatile obj)
{
  /*atomic exchange instruction*/
  __asm__ __volatile__ ("xchgl %1, %0"
			:"=r" (reg), "=m" (*obj)
			:"r" (reg), "m" (*obj));
  return (reg);
}

int main(int argc, char **argv)
{
  register double width, localsum;
  register int intervals, i;
  register int shmid;
  register int iproc = 0;
  
  /*allocate shared memory*/
  shmid = shmget(IPC_PRIVATE, 
		 sizeof(struct shared),
		 (IPC_CREAT | 0600));
  shared = ((volatile struct shared *) shmat(shmid, 0, 0));
  shmctl(shmid, IPC_RMID, 0);
  
  /*initialize*/
  shared->pi = 0.0;
  shared->lock = 0;

  /*fork*/
  if (!fork()) ++iproc;
  
  /*get the number of intervals*/
  intervals = atoi(argv[1]);
  width = 1.0 / intervals;
  
    
  /*do the local computations*/
  localsum = 0;
  for (i=iproc; i<intervals; i+=2) {
    register double x = (i + 0.5) * width;
    localsum += 4.0 / (1.0 + x * x);
  }
  localsum *= width;
  
  /*atomic lock, add, unlock...*/
  while (xchg((iproc + 1), (&shared->lock))) ;
  shared->pi+=localsum;
  shared->lock=0;

  /*terminate child*/
  if (iproc == 0) {
    wait(NULL);
    printf("Estimation of PI is %f\n", shared->pi);
  }
  
  return 0;
}
