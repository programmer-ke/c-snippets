#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define LIMIT 1000000

volatile int count = 0;
pthread_mutex_t count_lock; 

void *process(void *arg)
{
  int procid = *((char *) arg) - '0';

  while (count < LIMIT) {
    pthread_mutex_lock(&count_lock);
    count++;
    printf("Thread %d updated count: %d\n", procid, count);
    pthread_mutex_unlock(&count_lock);
    usleep(30);
  }
  
  printf("Thread %d exiting..\n", procid);
  return NULL;
}

int main(int argc, char **argv) 
{
  pthread_t thread0, thread1, thread2;
  void *retval;
  
  pthread_mutex_init(&count_lock, NULL);
  int i;
  
  if (pthread_create(&thread0, NULL, process, "0") ||
      pthread_create(&thread1, NULL, process, "1") ||
      pthread_create(&thread2, NULL, process, "2")) {
    fprintf(stderr, "Cannot create thread\n");
    exit(1);
  }
  
  if (pthread_join(thread0, &retval) ||
      pthread_join(thread1, &retval) ||
      pthread_join(thread2, &retval)) {
    fprintf(stderr, "Thread join failed");
    exit(1);
  }
  return 0;
}
