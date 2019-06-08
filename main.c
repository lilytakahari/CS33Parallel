#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>

#include "utils.h"

long old[DIM*DIM*DIM];

long __attribute__ ((noinline)) gimmie_the_func() {
  return 58113.3;
}

long __attribute__ ((noinline)) we_need_the_func() {
  return 823421.3;
}

int main( int argc, const char* argv[] )
{
  int i,j,k;
  int u;
  struct timespec start, finish;
  double seqDelay, parDelay;

  for (i=0; i<10; i++)
    histogrammy[i]=0;

  for (i=0; i<DIM; i++)
    for (j=0; j<DIM; j++)
      for (k=0; k<DIM; k++)
        {
          original[i*DIM*DIM+j*DIM+k]=(long)(random()%1000);
        }

  for (i=1; i<DIM-1; i++) {
    for (j=1; j<DIM-1; j++) {
      for (k=1; k<DIM-1; k++) {
        u=(original[i*DIM*DIM+j*DIM+k]/100);
        if (u>=9) u=9;
        if (u<=0) u=0;
        histogrammy[u]++;
      }
    }
  }

  for (i=0; i<10; i++)
    printf("HISTO[%d]:%d\n",i,histogrammy[i]);

  for (i=0; i<10; i++)
    histogrammy[i]=0;
  
  clock_gettime(CLOCK_MONOTONIC, &start);
  work_it_seq(original, old);
  clock_gettime(CLOCK_MONOTONIC, &finish);

  seqDelay = (finish.tv_sec - start.tv_sec);
  seqDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  for (i=0; i<10; i++)
    printf("NEWHISTO[%d]:%d\n",i,histogrammy[i]);

  printf("Sequential version took %lf time units\n",seqDelay);

  for (i=0; i<10; i++)
    histogrammy[i]=0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  work_it_par(original, new);
  clock_gettime(CLOCK_MONOTONIC, &finish);

  parDelay = (finish.tv_sec - start.tv_sec);
  parDelay += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  for (i=0; i<10; i++)
    printf("NEWPARHISTO[%d]:%d\n",i,histogrammy[i]);


  printf("Parallel version took %lf time units\n", parDelay);

  printf("This resulted in a %fx speed-up\n", (float)(seqDelay / parDelay));
  printf("Ending the parallelization test\n");

  /* TESTING STRUCTURE*/

  /*
    Make sure you include <unistd.h>
  */
  printf("\nRunning some extra tests to make sure everything is right...\n");
  printf("If this is taking way too long, you probably have a lot of errors so just quit\nby pressing CTRL-C a lot of times!");
  printf("\nNote: Please delete the fail.txt directory before testing if you think its reporting some nonsense\n");
  char cwd[60];
  char filePath[120];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror("getcwd() error");
    return 1;
  }
  sprintf(filePath, "%s/fail.txt",cwd);
  remove(filePath);
  FILE* fp = fopen (filePath,"w");
  long numWrong=0;
  int iScale, jScale;
  for (i=1; i<DIM-1; i++) {
    iScale = i *DIM*DIM;
    for (j=1; j<DIM-1; j++) {
      jScale = j*DIM;
      for (k=1; k<DIM-1; k++) {
	if(old[iScale+jScale+k] != new[iScale+jScale+k]){
	  fprintf (fp, "(%d, %d, %d) Should be: %ld\tbut is: %ld\n", i , j , k, old[iScale+jScale+k], new[iScale+jScale+k]);
	  ++numWrong;
	}
      }
    }
  }
  if(numWrong != 0){
    double pt = (numWrong / (DIM*DIM*DIM)) * 100;
    printf("\n\nATTENTION: There were some errors in the parallel version\n");
    printf("About %lf % of your matrix was wrong\nCheck the newly created fail.txt file to see which elements were wrong",pt);
  }else{
    printf("\nEverything looks good. Sequential and Parallel matrices match!\n");
  }

  /* TESTING STRUCTURE*/

  return 0;
}
