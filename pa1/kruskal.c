/*
 * kruskal.c - 
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

//GLOBAL
int flag;

// Note the unsigned short
struct edge {
  unsigned short v_1;
  unsigned short v_2;
  double weight;
};

typedef struct edge edge;

/* Test Functions START */

void print_list(edge *list, int n){
  int i;
  for (i = 0; i < n; i++){
    edge e = list[i];
    if (flag == 1)
      printf("#%d has weight %f and nodes %d and %d\n",i, e.weight, e.v_1, e.v_2);
  }
}

/* Test Functions END */

// n choose 2
int n_choose_2(int n)
{
  return (n*(n-1))/2;
}

void bottom_up_merge(edge *A, edge *B, int left, int right, int end){
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
  int i;

  // Initial copy work to sorted
  for (i = 0; i < n; i++){
    //printf("%d weighs %f\n", i, work[i].weight);
    sorted[i] = work[i];
  }

  // Make successively longer sorted runs of length 2, 4, 8, 16...
  for (width = 1; width < n; width = width << 1){
    // sorted is full of runs of length width
    for (i = 0; i < n; i = i + (width << 1)){
      /* merge two runs: A[i:i+width-1] and A[i+width:i+2*width-1] to B[] */
      /*  or copy A[i:n-1] to B[] ( if(i+width >= n) ) */
      bottom_up_merge(sorted, work, i, MIN(i+width, n), MIN(i+2*width, n));
    }

    // Copy work to sorted
    for (i = 0; i < n; i++){
      //printf("%d weighs %f\n", i, work[i].weight);
      sorted[i] = work[i];
    }
  }
}

double kruskal(edge *edgelist, node **vertices, int numedges)
{
  //Sort edges 
  edge *sorted = (edge *)malloc(numedges*sizeof(edge));
  bottom_up_sort(sorted, edgelist, numedges);

  print_list(sorted, numedges);
  
  int v1, v2, i;
  double wt;
  double total_weight = 0;
  
  //test
  double max = 0;

  for (i=0; i<numedges; i++)
    {
      v1 = edgelist[i].v_1;
      v2 = edgelist[i].v_2;
      wt = edgelist[i].weight;
  	
      if (find(vertices[v1]) != find(vertices[v2]))
  	{
	  printf("Adding edge (%d, %d) of weight %f\n",v1,v2,wt);
	  if (max < wt)
	    max = wt;
	  total_weight += wt;
	  dj_union(vertices[v1],vertices[v2]);
  	}
    }
  printf("max chosen weight is: %f\n", max);
}

double threshold(int numpoints, int dimension)
{
	if (numpoints > 10000)
		return 0.2;
	else
		return 1.0;
}

//This is the 0 dimension case with random weighted edges between each node
int kruskal_rand_wts (int numpoints)
{
  int numedges = n_choose_2(numpoints);
  double thresh = threshold(numpoints,0);
  
  node **vertices = (node **)malloc(numedges*sizeof(node *));
  edge *full_edgelist = (edge *)malloc(numedges*sizeof(edge));
    
  int i, j, k;
  k=0;
  srand(time(NULL)); //seeds random generator with current system time
  // Mark-Set
  for (i=0; i<numpoints; i++)
    {
      double w;
      
      // IMPLEMENT ME: 
      vertices[i] = makeset(i);
      for (j=0; j<numpoints; j++)
	{
	  if (i < j)
	    {
	      w = ((double)rand()/ (double)(RAND_MAX));
	  	  
	      //we can throw away edges here
	      if (w < thresh){
	      	full_edgelist[k].v_1 = i;
	      	full_edgelist[k].v_2 = j;
			full_edgelist[k].weight = w;
			k++;
	      }
	    }
	}
    }
  
  // Compress
  numedges = k;
  edge *edgelist = (edge *)malloc(numedges*sizeof(edge));
  for (i = 0; i < numedges; i++){
    edgelist[i] = full_edgelist[i];
  }
  free(full_edgelist);

  if (flag == 1){
    print_list(edgelist, numedges);
    printf("-----\n");
  }
  
  double total_wt = kruskal(edgelist,vertices,numedges);
}

int kruskal_rand_points(int numpoints, int dimension)
{
  double points[numpoints][dimension];
  int numedges = n_choose_2(numpoints);
  double thresh = threshold(numpoints,dimension);
  
  node **vertices = (node **)malloc(sizeof(node *)*numpoints);
  edge *full_edgelist = (edge *)malloc(sizeof(edge)*numedges);
  
  int i, j, k, l;
  l=0;
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
	      
	      if (dist < thresh)
	      {
	      	full_edgelist[l].v_1 = i;
	      	full_edgelist[l].v_2 = j;
	      	full_edgelist[l].weight = dist;
	      		      
	      	l++;
	      	printf("%d %d - %f\n",i,j,dist);
	      }
	    }
	}
    }
    
  numedges = l;
  edge *edgelist = (edge *)malloc(sizeof(edge)*numedges);
  
  for (i = 0; i < numedges; i++){
    edgelist[i] = full_edgelist[i];
  }
  free(full_edgelist);
  
    if (flag == 1){
    print_list(edgelist, numedges);
    printf("-----\n");
  }
    
  double total_weight = kruskal(edgelist, vertices, numedges);
}

int main (int argc, char **argv)
{
  if (argc != 5) {
    printf("Usage: 0 numpoints numtrials dimension\n");
    return 0;
  }
    
  flag = atoi(argv[1]);
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
      kruskal_rand_wts(numpoints);
  if (dimension > 1 && dimension < 5)
      kruskal_rand_points(numpoints,dimension);
}