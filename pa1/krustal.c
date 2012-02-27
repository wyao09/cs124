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
#include <math.h>
#include "dj_set.h"

struct edge {
  // QUESTION: are v_1 and v_2 necessary? can we just use prev->id?
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

// 
edge * sort(edge *edgelist, int length){
  
}

//This is the 0 dimension case with random weighted edges between each node
int krustal_rand_wts (int numpoints)
{
  // QUESTION: Use integer values for faster run time?
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
      
      // IMPLEMENT ME: 
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
	  		
			// Prepend
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
	node *vertices[numpoints];
	
	edge *edgelist = NULL;
	edge *tmp;
	
	int i, j, k;
	double dist;
	
	srand(time(NULL));
	
	for (i=0; i<numpoints; i++)
	{
		vertices[i] = makeset(i);
		for (j=0; j<dimension; j++)
		{
			points[i][j] = ((double)rand()/ (double)(RAND_MAX));
			printf("%f\t",points[i][j]);
		}
		printf("\n");
	}
	
	for (i=0; i<numpoints; i++)
	{
		for (j=0; j<numpoints; j++)
		{
			if (i < j)
			{
				dist = 0;
				for (k=0; k<dimension; k++)
					dist += pow((points[j][k] - points[i][k]),2);
				dist = dist/k;
				dist = pow(dist, (1.0/k));
				printf("%d %d - %f\n",i,j,dist);
			}
		}
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
	if (dimension > 1 && dimension < 5)
	{
		krustal_rand_points(numpoints,dimension);
	}
}
