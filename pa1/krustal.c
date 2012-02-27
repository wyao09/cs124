/*
 * krustal.c - 
 *
 * usage: randmst 0 <numpoints> <numtrials> <dimension>
 *
 * assignment: cs124 pa1
 *
 * authors: Aidan Daly <aidan.daly@college.harvard.edu>
 *          Willie Yao <wyao13@college.harvard.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dj_set.h"

//This is the 0 dimension case with random weighted edges between each node
int krustal_rand_wts (int numpoints)
{
  //Use integer values for faster run time?
  double weights[numpoints][numpoints];
  node *vertices[numpoints];
    
  int i, j;
  srand(time(NULL)); //seeds random generator with current system time
    
  for (i=0; i<numpoints; i++)
    {
      vertices[i] = makeset(i);
      for (j=0; j<numpoints; j++)
      {
      	if (i < j)
      	{
	  		weights[i][j] = ((double)rand()/ (double)(RAND_MAX));
	  		weights[j][i] = weights[i][j];
	  	}
	  	if (i==j)
	  		weights[i][j] = 0;
	  	printf("%f\t",weights[i][j]);
	  }
	  printf("\n");
    }
    
  //Sort edges
    
  //for edges in increasing order
}

int main (int argc, char **argv)
{
  if (argc != 5) {
    printf("Usage: 0 numpoints numtrials dimension\n");
    return 0;
  }
    
  int flag = atoi(argv[1]);
  int numpoints = atoi(argv[2]);
  int numtrials = atoi(argv[3]);
  int dimension = atoi(argv[4]);
    
  if (numpoints <= 0 || numtrials <= 0 || dimension < 0)
    {
      printf("numpoints, numtrials, dimension must be non-negative integers");
      return 0;
    }
    
  // Case for Random Weights
  if (dimension == 0)
    {
      krustal_rand_wts(numpoints);
    }
}
