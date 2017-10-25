#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 10000
#define RUNS 100000

int marius[MAX][MAX];

double benchmark();

int main()
{
  int i,j;
  for(i=0;i<MAX;i++)
  {
    for(j=0;j<MAX;j++)
    {
      marius[i][j]=rand();
    }
  }

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
  srand(time(NULL));

  double startTime = (float)clock()/CLOCKS_PER_SEC;

  suma_coloane();

  double endTime = (float)clock()/CLOCKS_PER_SEC;

  double timeElapsed = endTime - startTime;

  return timeElapsed;
}

int suma_linii()
{
  int i,j;
  int sum=0;
  for(i=0;i<MAX;i++)
  {
    for(j=0;j<MAX;j++)
      sum+=marius[i][j];
  }
  return sum;
}

int suma_coloane()
{
  int i,j;
  int sum=0;
  for(i=0;i<MAX;i++)
  {
    for(j=0;j<MAX;j++)
      sum+=marius[j][i];
  }
  return sum;
}
