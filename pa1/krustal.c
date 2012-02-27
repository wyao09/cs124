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
  int v_1;
  int v_2;
  double weight;
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
edge * sort(edge *edgelist){
  
}

//This is the 0 dimension case with random weighted edges between each node
int krustal_rand_wts (int numpoints)
{
  // QUESTION: Use integer values for faster run time?
  double weights[numpoints][numpoints];
  int numedges = fact(numpoints)/(fact(numpoints-2)*2);
  
  node *vertices[numpoints];
  edge edgelist[fact(numpoints-1)];
    
  int i, j, k;
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
	  	  	k = i*numpoints+j;
	  	  
			edgelist[k].v_1 = i;
			edgelist[k].v_2 = j;
			edgelist[k].weight = w;
    	}
      }
    }
    
  //Sort edges
    
  //for edges in increasing order
}

int krustal_rand_points(int numpoints, int dimension)
{
  double points[numpoints][dimension];
  node *vertices[numpoints];
	
  edge edgelist[fact(numpoints-1)];
	
  int i, j, k;
  double dist, tmp;
	
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
	      dist = 0.0;
	      for (k=0; k<dimension; k++)
	      {
	      	tmp = points[i][k]-points[j][k]; 
			dist += pow(tmp,2);
		  }
	      dist = dist/k;
	      dist = pow(dist, (1.0/k));
	      
	      edgelist[i*numpoints+j].v_1 = i;
	      edgelist[i*numpoints+j].v_2 = j;
	      edgelist[i*numpoints+j].weight = dist;
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
