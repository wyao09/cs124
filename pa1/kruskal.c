/*
 * kruskal.c - 
 *
 * usage: randmst <opcode> <numpoints> <numtrials> <dimension>
 * opcode 0 = test for average tree size
 * opcode 1 = test for maximum edge weight
 * opcode 2 = test for average Kruskals run time (omits generation)
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
#define BOUND 1
#define TIMER 2

// Empirically tested for our specific machine to push limits
#define MAXPOINTS 32678
#define MAXEDGES 8000000000000

//GLOBAL
int flag;
int edge_counter = 0;

struct edge {
  unsigned int v_1;
  unsigned int v_2;
  double weight;
};

typedef struct edge edge;

struct list {
  edge e;
  struct list *next;
};

typedef struct list list; 

/* Test Functions START*/

void print_list(edge *list, int n) {
  int i;
  for (i = 0; i < n; i++){
    edge e = list[i];
  }
}

/* Helper Functions START */

int n_choose_2(int n){
  return (n*(n-1)) >> 1;
}

double threshold(int numpoints, int dimension) {
  if (dimension == 0){
    // Tighter bound for case of n = 32678 or smaller
    if(numpoints <=32678)
      return 1.5/log2(numpoints) -.09;
    return 1.0/log2(numpoints);
  }
  else
    return ((double)(dimension))/log2(numpoints);
}

/* Merge Functions START */

void bottom_up_merge(edge *A, edge *B, int left, int right, int end) {
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

void bottom_up_sort(edge *sorted, edge *work, int n) {
  int width;
  int i;

  // Initial copy work to sorted
  for (i = 0; i < n; i++){
    sorted[i] = work[i];
  }

  // Make successively longer sorted runs of length 2, 4, 8, 16...
  for (width = 1; width < n; width = width << 1){
    // Sorted is full of runs of length width
    for (i = 0; i < n; i = i + (width << 1)){
      bottom_up_merge(sorted, work, i, MIN(i+width, n), MIN(i+(width << 1), n));
    }

    // Copy work to sorted
    for (i = 0; i < n; i++){
      sorted[i] = work[i];
    }
  }
}

/* Kruskal Functions START */

double kruskal(edge *edgelist, node **vertices, int numedges) {
  //Sort edges 
  edge *sorted = (edge *)malloc(numedges*sizeof(edge));
  bottom_up_sort(sorted, edgelist, numedges);
  
  int v1, v2, i;
  double wt;
  double total_weight = 0;
  double max = 0;

  for (i=0; i<numedges; i++)
    {
      v1 = edgelist[i].v_1;
      v2 = edgelist[i].v_2;
      wt = edgelist[i].weight;
  	
      if (find(vertices[v1]) != find(vertices[v2]))
  	{
	  if (flag == BOUND && max < wt)
	    max = wt;
	  
	  total_weight += wt;
	  dj_union(vertices[v1],vertices[v2]);
  	}
    }
  
  // Testing upperbound
  if (flag == BOUND)
    return max;
	
  free(sorted);

  return total_weight;
}



//This is the 0 dimension case with random weighted edges between each node
//Returns total weight of tree
double kruskal_rand_wts (int numpoints) {
  int numedges = n_choose_2(numpoints);
  double thresh = threshold(numpoints,0);
  
  node **vertices = (node **)malloc(numpoints*sizeof(node *));
  edge *full_edgelist;
  if (numpoints > MAXPOINTS)
    full_edgelist = (edge *)malloc(MAXEDGES*sizeof(edge));
  else
    full_edgelist = (edge *)malloc(numedges*sizeof(edge));

  int i, j, k = 0;
  
  struct timeval t1;
  gettimeofday(&t1, NULL);
  srand(t1.tv_usec * t1.tv_sec);
  
  // Mark-Set
  for (i=0; i<numpoints; i++)
    {
      double w;
       
      vertices[i] = makeset(i);
      for (j=0; j<numpoints; j++)
	{
	  if (i < j)
	    {
	      w = ((double)rand()/ (double)(RAND_MAX));
	  	  
	      // Throw away edges
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

  // timer start
  struct timeval t3, t4;
  if (flag == TIMER){
    edge_counter += numedges;
    gettimeofday(&t3, NULL); 
  }

  double total_wt = kruskal(edgelist,vertices,numedges);
  
  // timer end
  if (flag == TIMER){
    gettimeofday(&t4, NULL);
    return (pow(10,6)*t4.tv_sec + t4.tv_usec) - (pow(10,6)*t3.tv_sec + t3.tv_usec);
  }

  for (i=0; i<numpoints; i++)
    free(vertices[i]);
  free(edgelist);
  free(vertices);

  if (flag == TIMER){
    return (pow(10,6)*t4.tv_sec + t4.tv_usec) - (pow(10,6)*t3.tv_sec + t3.tv_usec);
  }

  return total_wt;
}


//Returns total weight of tree
double kruskal_rand_points(int numpoints, int dimension) {
  double points[numpoints][dimension];
  int numedges = n_choose_2(numpoints);
  double thresh = threshold(numpoints,dimension);

  node **vertices = (node **)malloc(numpoints*sizeof(node *));
  edge *full_edgelist;
  if (numpoints > MAXPOINTS){
    full_edgelist = (edge *)malloc(MAXEDGES*sizeof(edge));
  }
  else
    full_edgelist = (edge *)malloc(numedges*sizeof(edge));
    
  int i, j, k, l;
  l=0;
  double dist, tmp;
	
  struct timeval t1;
  gettimeofday(&t1, NULL);
  srand(t1.tv_usec * t1.tv_sec);
	
  for (i=0; i<numpoints; i++)
    {
      vertices[i] = makeset(i);
      for (j=0; j<dimension; j++)
	{
	  points[i][j] = ((double)rand()/ (double)(RAND_MAX));
	}
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
	      dist = pow(dist, (1.0/2));
	      
	      if (dist < thresh)
		{
		  full_edgelist[l].v_1 = i;
		  full_edgelist[l].v_2 = j;
		  full_edgelist[l].weight = dist;
	      		      
		  l++;
		}
	    }
	}
    }

  // compress
  numedges = l;
  edge *edgelist = (edge *)malloc(sizeof(edge)*numedges);
  
  for (i = 0; i < numedges; i++){
    edgelist[i] = full_edgelist[i];
  }
  free(full_edgelist);
  
  // timer start
  struct timeval t3, t4;
  if (flag == TIMER){
    edge_counter += numedges;
    gettimeofday(&t3, NULL); 
  }  
  
  double total_wt = kruskal(edgelist,vertices,numedges);
  
  // timer end
  if (flag == TIMER){
    gettimeofday(&t4, NULL);
    return (pow(10,6)*t4.tv_sec + t4.tv_usec) - (pow(10,6)*t3.tv_sec + t3.tv_usec);
  }

  for (i=0; i<numpoints; i++)
    free(vertices[i]);
  free(edgelist);
  free(vertices);

  if (flag == TIMER){
    return (pow(10,6)*t4.tv_sec + t4.tv_usec) - (pow(10,6)*t3.tv_sec + t3.tv_usec);
  }

  return total_wt;
}

/* MAIN */

int main (int argc, char **argv) {
  if (argc != 5) {
    printf("Usage: randmst <opcode> <numpoints> <numtrials> <dimension>\n");
    return 0;
  }
    
  flag = atoi(argv[1]);
  int numpoints = atoi(argv[2]);
  int numtrials = atoi(argv[3]);
  int dimension = atoi(argv[4]);

  if (numpoints <= 0 || numtrials <= 0 || dimension < 0) {
    printf("numpoints, numtrials, dimension must be non-negative integers");
    return 0;
  }
  
  // Test Upperbound
  if (flag == BOUND) {
    int i;
    double max;
    double tmp;

    for (i=0; i<numtrials; i++){
      if (dimension == 0){
      	tmp = kruskal_rand_wts(numpoints);
	if (tmp > max)
	  max = tmp;
      }
      
      if (dimension > 1 && dimension < 5){
	tmp = kruskal_rand_points(numpoints,dimension);
	if (tmp > max)
	  max = tmp;
      }
    }

    printf("%d,%d,%d,%f\n", 
	   numtrials, dimension, numpoints, max);
  }

  else{
    // Case for Random Weights
    int i;
    double wt = 0.0;
    double time = 0.0;
  
    struct timeval t1, t2;
    srand(t1.tv_usec * t1.tv_sec);
  	
    for (i=0; i<numtrials; i++)
      {
  		if (dimension == 0)
  		{
  			gettimeofday(&t1, NULL); 
	 		wt += kruskal_rand_wts(numpoints);
	 		gettimeofday(&t2, NULL);
	 		time += (pow(10,6)*t2.tv_sec + t2.tv_usec) - (pow(10,6)*t1.tv_sec + t1.tv_usec);
	 	}
  		else if (dimension > 1 && dimension < 5)
  		{
			gettimeofday(&t1, NULL); 
			wt += kruskal_rand_points(numpoints,dimension);
	 		gettimeofday(&t2, NULL);
	 		time += (pow(10,6)*t2.tv_sec + t2.tv_usec) - (pow(10,6)*t1.tv_sec + t1.tv_usec);
	  	}
      }
    // Print average edges kept (the E in O(E log V) )
    if (flag == TIMER){
      printf("%d,", (edge_counter/numtrials));
    }
    printf("%d,%d,%f,%f\n", dimension, numpoints , (wt/numtrials), (time/numtrials));
  }
}
