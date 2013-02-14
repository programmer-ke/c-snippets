#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile double pi = 0.0;
pthread_mutex_t pi_lock;
volatile double intervals;

void * process(void *arg)
{
  register double width, localsum;
  register int i;
  register int iproc = (*((char *) arg) - '0');
  
  /*set width*/
  width = 1.0 / intervals;
  
  localsum = 0;
  for (i=iproc; i<intervals; i+=2) {
    register double x = (i + 0.5) * width;
    localsum += 4.0 / (1.0 + x * x);
  }
  localsum *= width;
  
  pthread_mutex_lock(&pi_lock);
  pi += localsum;
  pthread_mutex_unlock(&pi_lock);
 
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_t thread0, thread1;
  void *retval;
  
  intervals = atoi(argv[1]);
  
  pthread_mutex_init(&pi_lock, NULL);
  
  if (pthread_create(&thread0, NULL, process, "0") ||
      pthread_create(&thread1, NULL, process, "1")) {
    fprintf(stderr, "%s: cannot make thread\n", argv[0]);
    exit(1);
  }

  if (pthread_join(thread0, &retval) ||
      pthread_join(thread1, &retval)) {
    fprintf(stderr, "%s: thread join failed\n", argv[0]);
    exit(1);
  }

  printf("Estimation of PI is %f\n", pi);
  
  exit(0);
}
