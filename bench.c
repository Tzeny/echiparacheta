#include <stdio.h>
#include <time.h>

#define MAX 1000000
#define RUNS 100000

double benchmark();

int main()
{
  int i;
  double average=0;
  double total=0;
  for(i=0;i<RUNS;i++)
  {
    average+=benchmark();
  }
  total=average;
  average /= RUNS;

  printf("\n\nAverage time elapsed: %lf \n Runs: %d \n", average, RUNS);
  printf("\nTotal time elapsed: %lf\n\n", total);
}

double benchmark()
{
  double startTime = (float)clock()/CLOCKS_PER_SEC;

  int i;
  int a=0;
  for(i=0;i<MAX;i++)
  {
    if(check_prime(i)==1)
      a++;
  }

  //printf("\n%d numere prime de la 0 la %d\n\n", a, MAX);

  /* Do work */

  double endTime = (float)clock()/CLOCKS_PER_SEC;

  double timeElapsed = endTime - startTime;

  return timeElapsed;
}

int check_prime(int a)
{
  int i;
  for(i=2;i<a/2;i++)
  {
    return 0;
  }
  return 1;
}
