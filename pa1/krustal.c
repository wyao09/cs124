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

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

struct edge {
  int v_1;
  int v_2;
  double weight;
};

typedef struct edge edge;

/* Test Functions START */

void print_list(edge *list, int n){
  int i;
  for (i = 0; i < n; i++){
    edge e = list[i];
    printf("#%d has weight %f and nodes %d and %d\n",i, e.weight, e.v_1, e.v_2);
  }
}

/* Test Functions END */

int fact(int x)
{
  int i;
  int j=1;
  for (i=x; i>1; i--)
    j = j*i;
  return j;		
}

void bottom_up_merge(edge A[], edge B[], int left, int right, int end){
  int i0 = left;
  int i1 = right;
  int j;
 
  // while there are elements on either side of lists
  for (j = left; j < end; j++){
    // if left list head exists and is <= existing right list head
    if (i0 < right && (i1 >= end || A[i0].weight <= A[i1].weight)){
      B[j] = A[i0];
      i0 += 1;
    }
    else{
      B[j] = A[i1];
      i1 += 1;
    }
  }
}

void bottom_up_sort(edge *sorted, edge *work, int n){
  int width;
  // Make successively longer sorted runs of length 2, 4, 8, 16...
  for (width = 1; width < n; width = width << 1){
    int i;
    // sorted is full of runs of length width
    for (i = 0; i < n; i = i + (width << 1)){
      /* merge two runs: A[i:i+width-1] and A[i+width:i+2*width-1] to B[] */
      /*  or copy A[i:n-1] to B[] ( if(i+width >= n) ) */
      bottom_up_merge(sorted, work, i, MIN(i+width, n), MIN(i+2*width, n));
    }

    // Copy work to sorted
    for (i = 0; i < n; i++){
      sorted[i] = work[i];
    }
  }
}




//This is the 0 dimension case with random weighted edges between each node
int krustal_rand_wts (int numpoints)
{
  // QUESTION: Use integer values for faster run time?
  double weights[numpoints][numpoints];
  int numedges = fact(numpoints)/(fact(numpoints-2)*2);
  
  
  node *vertices[numpoints];
  edge *edgelist = (edge *)malloc(numedges*sizeof(edge));
    
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
			
			printf("%d, %d - %f\n",edgelist[k].v_1,edgelist[k].v_2,edgelist[k].weight);
    	}
      }
    }
    
  //Sort edges
  print_list(edgelist, numedges);
  printf("\n");
  edge *sorted = (edge *)malloc(numedges*sizeof(edge));
  bottom_up_sort(sorted, edgelist, numedges);
  print_list(sorted, numedges);  
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
