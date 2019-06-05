//OpenMP version.  Edit and submit only this file.
/* Enter your details below
 * Name : Lily Takahari
 * UCLA ID : 305108348
 * Email : lilytakahari@gmail.com
 */

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "utils.h"

void work_it_par(long *old, long *new) {
  int i, j, k, ii, jj, kk, u, v, w;
  long aggregate=1.0;
  
  /* multiple histogram values to enable parallelism */
  long h0 = 0, h1 = 0, h2 = 0, h3 = 0, h4 = 0;
  long h5 = 0, h6 = 0, h7 = 0, h8 = 0, h9 = 0;
  
  /* code motion: function values, common expressions */
  long needVal = we_need_the_func();
  long gimmieVal = gimmie_the_func();
  const long dimSq = DIM * DIM;
  const long dim = DIM - 1;
  /* tileSize = 6 because DIM = 500, but mostly loop 498
   * times, and 498 can be divided by 6 */
  const int tileSize = 6;
  const dimTile = dim - tileSize;
  
#pragma omp parallel for private(i,j,k) reduction(+:aggregate)
  for (i=1; i<dim; i++) {
    /* accumulator variable to reduce multiplication */
    long jAcc = (i<<4)*15625 + DIM;
    for (j=1; j<dim; j++) {
      /* 6 x 1 loop unrolling */
      for (k=1; k<=dimTile; k+=tileSize) {
	long kStart = jAcc + k;
        long comp0 = old[kStart] * needVal;
        aggregate+= comp0 / gimmieVal;
	long comp1 = old[kStart+1] * needVal;
        aggregate+= comp1 / gimmieVal;
	long comp2 = old[kStart+2] * needVal;
        aggregate+= comp2 / gimmieVal;
	long comp3 = old[kStart+3] * needVal;
        aggregate+= comp3 / gimmieVal;
	long comp4 = old[kStart+4] * needVal;
        aggregate+= comp4 / gimmieVal;
	long comp5 = old[kStart+5] * needVal;
        aggregate+= comp5 / gimmieVal;
      }
      jAcc += DIM;
    }
  }

  printf("AGGR:%ld\n",aggregate);
  
#pragma omp parallel for private(ii,jj,kk,i,j,k,u,v)
  for (ii=1; ii<=dimTile; ii+=tileSize) {
    for (jj=1; jj<=dimTile; jj+=tileSize) {
      for (kk=1; kk<=dimTile; kk+=tileSize) {
	/* loop tiling, tile size 6 */
	/* more accumulator variables */
	long iAcc = (ii<<4)*15625;
	for (i = ii; i < ii + tileSize; i++, iAcc += dimSq) {
          long jAcc = iAcc + (jj * DIM);
          for (j = jj; j < jj + tileSize; j++, jAcc += DIM) {
            long kAcc = jAcc + kk;
            for (k = kk; k < kk + tileSize; k++, kAcc++) {
	      new[jAcc+k]=0;
	      
	      long oldU = kAcc - dimSq;
	      for (u=-1; u<=1; u++) {
		long oldV = oldU - DIM;
		for (v=-1; v<=1; v++) {
		  new[kAcc] += (old[oldV-1] + old[oldV]) + old[oldV+1];
		  oldV += DIM;
		}
		oldU += dimSq;
	      }	    
	      new[jAcc+k]/=27;
	    }
	    jAcc += DIM;
	  }
	}
      }
    }
  }
  /* all those extra lines, a necessary consequence of parallelism */
#pragma omp parallel for private(i,j,k,u) reduction(+:h0,h1,h2,h3,h4,h5,h6,h7,h8,h9)
  for (i=1; i<dim; i++) {
    long jAcc = (i<<4)*15625 + DIM;
    for (j=1; j<dim; j++) {
      for (k=1; k<dim; k++) {
        u=(new[jAcc+k]>>2)/25;
        if (u<=0) u=0;
        if (u>=9) u=9;
	switch (u) {
	case 0: h0++; break;
	case 1: h1++; break;
	case 2: h2++; break;
	case 3:	h3++; break;
	case 4:	h4++; break;
	case 5:	h5++; break;
	case 6:	h6++; break;
	case 7:	h7++; break;
	case 8:	h8++; break;
	case 9:	h9++; break;
	}
      }
      jAcc += DIM;
    }
  }
  histogrammy[0] = h0;
  histogrammy[1] = h1;
  histogrammy[2] = h2;
  histogrammy[3] = h3;
  histogrammy[4] = h4;
  histogrammy[5] = h5;
  histogrammy[6] = h6;
  histogrammy[7] = h7;
  histogrammy[8] = h8;
  histogrammy[9] = h9;
}
