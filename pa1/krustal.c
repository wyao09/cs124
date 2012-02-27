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

struct edge {
	int v_1;
	int v_2;
	double weight;
	struct edge *prev;
	struct edge *next;
};

typedef struct edge edge;

int fact(int x)
{
	int i;
	int j=1;
	for (i=x; i>1; i--)
		j = j*i;
	return j;		
}


//This is the 0 dimension case with random weighted edges between each node
int krustal_rand_wts (int numpoints)
{
  //Use integer values for faster run time?
  double weights[numpoints][numpoints];
  int numedges = fact(numpoints)/(fact(numpoints-2)*2);
  
  node *vertices[numpoints];
  edge *edgelist = NULL;
  edge *tmp;
    
  int i, j;
  srand(time(NULL)); //seeds random generator with current system time
   
  // Mark-Set
  for (i=0; i<numpoints; i++)
    {
      int k = i*numpoints+j;
      double w;
      
      vertices[i] = makeset(i);
      for (j=0; j<numpoints; j++)
      {
      	if (i < j)
      	{
	  		w = ((double)rand()/ (double)(RAND_MAX));
	  		
	  		tmp = (edge *) malloc(sizeof(edge));
	  		tmp->v_1 = i;
	  		tmp->v_2 = j;
	  		tmp->weight = w;
	  		
	  		if (edgelist != NULL)
	  			edgelist->prev = tmp;
	  		tmp->next = edgelist;
	  		edgelist = tmp;
	  	}
	  }
    }
    
  tmp = edgelist;
  while (tmp != NULL)
  {
  	printf("(%d, %d) - %f\n",tmp->v_1, tmp->v_2, tmp->weight);
  	tmp = tmp->next;
  }
    
  //Sort edges
    
  //for edges in increasing order
}

int krustal_rand_points(int numpoints, int dimension)
{
	double points[numpoints][dimension];
	int i, j;
	srand(time(NULL));
	
	for (i=0; i<numpoints; i++)
	{
		for (j=0; j<dimensions; j++)
			points[i][j] = ((double)rand()/ (double)(RAND_MAX));
	}
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
