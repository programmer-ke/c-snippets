#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
  register double width, sum;
  register int intervals;

  /*get the number of intervals*/
  intervals = atoi(argv[1]);
  width = 1.0 / intervals;
  
  /*do the computation*/
  sum = 0.0;
  int i;
  for (i=0; i<intervals; i++) {
    register double x = (i + 0.5) * width;
    sum += 4.0 / (1.0 + x * x);
  }
  sum *= width;
  
  printf("Estimation of PI is %f\n", sum);
  exit(0);
}
  
