/*
 * strassen.c - 
 *
 * usage: ./strassen 0 <dimension> <inputﬁle>
 *
 * opcode 0 = conventional, 1 = strassens
 *
 * assignment: cs124 pa2
 *
 * authors: Aidan Daly <aidan.daly@college.harvard.edu>
 *          Willie Yao <wyao13@college.harvard.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "strassen.h"

#define I 1
#define II 2
#define III 3
#define IV 4
#define INT 12

int dimension = 1;
int CUTOFF = 256;

int main(int argc, char **argv){
  if (argc != 4){
    printf("usage: ./strassen 0 <dimension> <inputﬁle>|r\n");
    return 1;
  }   

  // commandline input
  int dim = atoi(argv[2]);
  dimension = dim;
  int opcode = atoi(argv[1]);

  int **a, **b, **c;
  
  //Padding to nearest power of 2 for Strassen
  int dim_2 = dim;
  if (opcode == 1) {
  	int x = 0;
  	while (pow(2,x) < dim)
	  x++;
	dim_2 = pow(2,x);
  }
  a = m_malloc(dim_2);
  b = m_malloc(dim_2);
  c = m_malloc(dim_2);
  
  //Zeroes all entries
  int i,j;
  for (i=0; i<dim; i++) {
  	for (j=0; j<dim; j++) {
  		a[i][j] = 0; b[i][j] = 0; c[i][j] = 0;
  	}
  }

  // reads input matrices
  if (strcmp(argv[3],"r") != 0) {
  	FILE *fp;
  	fp = fopen(argv[3], "r");
  	if (read_matrices(dim, a, b, fp) == 0)
  		return 0;
  }
  // generates random matrices
  else {
  	rand_matrix(dim,a);
  	rand_matrix(dim,b);
  }

    
  /*print_matrix(dim, a);
  printf("\n");
  print_matrix(dim, b);
  printf("\n");*/
	
  struct timeval t1, t2;
  int time;
    
  srand(t1.tv_usec * t1.tv_sec);
  gettimeofday(&t1,NULL);
  
  if(opcode == 1){
    //printf("Strassen's algorithm - %d\n",dim_2);
    strassen(a, b, c, dim_2);
  }
  else if (opcode == 2)
  	transpose_conv(a,b,c,dim);
  else if (opcode > 5){
  	CUTOFF = opcode;
  	strassen(a,b,c,dim_2);
  }
  else
    conventional(a, b, c, dim);

  gettimeofday(&t2,NULL);
  time = (pow(10,6)*t2.tv_sec + t2.tv_usec) - (pow(10,6)*t1.tv_sec + t1.tv_usec);
  printf("%d\n",time);
  
  /*printf("Result:\n");
  print_matrix(dim, c);*/

  m_free(a, dim);
  m_free(b, dim);
  m_free(c, dim);

  return 0;
}

int read_matrices(int dim, int **a, int **b, FILE *fp)
{
  if (fp != NULL){
    char line[INT];
    int i = 0;
    int n;

    while( fgets(line, sizeof line, fp) != NULL){
      n = atoi(line);
      if(i < dim*dim){
	a[i/dim][i%dim] = n;
      }
      else{
	b[i/dim - dim][i%dim] = n;
      }
      i++;
    }
    fclose(fp);
    return 1;
  }
  else{
    printf("usage: ./strassen 0 <dimension> <inputﬁle>\n");
    return 0;
  }
}

/* generate random matrix */
// need to add argument that determines type of random matrix
void rand_matrix(int n, int **m){
  //Seeds random number generator with posix time
  struct timeval t1;
  gettimeofday(&t1, NULL);
  srand(t1.tv_usec * t1.tv_sec);
  
  int i, j;
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      m[i][j] = rand() % 3;
    }
  }
  return;
}

/* print matrix */
void print_matrix(int n, int **m){
  int i, j;
  for (i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      printf("%d ",m[i][j]);
    }
    printf("\n");
  }
  return;
}

/* transposes d by d square matrix m */ 
void transpose(int **m, int d){
	int i, j;
	int tmp=0;

	for (i=0; i<d; i++){
		for (j=i+1; j<d; j++){
			tmp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = tmp;
		}
	}
}

/* conventional matrix multiplication */
// assumes square matrices
void conventional(int **a, int **b, int **c, int d){
  int i, j, k;
  int tmp;

  // runs across a column
  for (i=0; i<d; i++){
    // runs across a row
    for (j=0; j<d; j++){
      tmp = 0;
      // calculate an element
      for (k=0; k<d; k++){
	tmp += a[i][k] * b[k][j];
      }
      c[i][j] = tmp;
    }
  }
}

/* tranpose matrix multiplication to avoid cache misses */
// assumes square matrices
void transpose_conv(int **a, int **b, int **c, int d){
	int i, j, k, tmp;

	transpose(b,d);
	
	for (i=0; i<d; i++){
		for (j=0; j<d; j++){
    		tmp = 0;
		    for (k=0; k<d; k++)
				tmp += a[i][k] * b[j][k];
      	c[i][j] = tmp;
    	}
  	}
  	//NOTE: As of now, restores b at end of function.
  	//Should we instead make a copy of b then free it?
  	transpose(b,d);
}

// can be optimized?
int **m_malloc(dim){
  int i;
  int** m = (int**)malloc(dim * sizeof(int*));     
  for (i=0;i<dim;++i)
    m[i] = (int*)malloc(dim * sizeof(int));
  return m;
}

void m_free(int **m, int dim){
  int i;
  for (i=0;i<dim;++i)
    free(m[i]);
  free(m);
  return;
}


/* Strassen's algorithm */


void sum(int **a, int **b, int **c, int d){
  int i,j;
  for (i=0; i<d; i++)
    for (j=0; j<d; j++)
      c[i][j] = a[i][j] + b[i][j];
}

//this assumes even dim
void sum_m(int **a, int **b, int **c, int new_d, int mode){
  int i,j;
  for (i=0; i<new_d; i++){
    for (j=0; j<new_d; j++){
      if(mode == I)
	c[i][j] = a[i][j] + b[i][j];
      else if(mode == II)
	c[i][j + new_d] = a[i][j] + b[i][j];
      else if(mode == III)
	c[i + new_d][j] = a[i][j] + b[i][j];
      else if(mode == IV)
	c[i + new_d][j + new_d] = a[i][j] + b[i][j];
    }
  }
}

void sub(int **a, int **b, int **c, int d){
  int i,j;
  for (i=0; i<d; i++)
    for (j=0; j<d; j++)
      c[i][j] = a[i][j] - b[i][j];
}

//this assumes even dim
void sub_m(int **a, int **b, int **c, int new_d, int mode){
  int i,j;
  for (i=0; i<new_d; i++){
    for (j=0; j<new_d; j++){
      if(mode == I)
	c[i][j] = a[i][j] - b[i][j];
      else if(mode == II)
	c[i][j + new_d] = a[i][j] - b[i][j];
      else if(mode == III)
	c[i + new_d][j] = a[i][j] - b[i][j];
      else if(mode == IV)
	c[i + new_d][j + new_d] = a[i][j] - b[i][j];
    }
  }
}

void strassen(int **a, int **b, int **c, int d){
  if (d <= CUTOFF){
  	transpose_conv(a,b,c,d);
  	return;
  }
  else if (d == 1){
    c[0][0] = a[0][0] * b[0][0];
    return;
  }

  int i, j;
  int new_dim = d/2; //this assume that n is even?

  int **a11, **a12, **a21, **a22;
  int **b11, **b12, **b21, **b22;
  //  int **c11, **c12, **c21, **c22;
  int **m1, **m2, **m3, **m4, **m5, **m6, **m7;
  int **t1, **t2;

  //a11 = m_malloc(new_dim);
  a12 = m_malloc(new_dim);
  //a21 = m_malloc(new_dim);
  a22 = m_malloc(new_dim);
  //b11 = m_malloc(new_dim);
  b12 = m_malloc(new_dim);
  //b21 = m_malloc(new_dim);
  b22 = m_malloc(new_dim);

  m1 = m_malloc(new_dim);
  m2 = m_malloc(new_dim);
  m3 = m_malloc(new_dim);
  m4 = m_malloc(new_dim);
  m5 = m_malloc(new_dim);
  m6 = m_malloc(new_dim);
  m7 = m_malloc(new_dim);
  t1 = m_malloc(new_dim);
  t2 = m_malloc(new_dim);

  // dividing into 4 sub matrices
  for (i=0; i<new_dim; i++){
    for (j=0; j<new_dim; j++){
      //a11[i][j] = a[i][j];
      a12[i][j] = a[i][j + new_dim];
      //a21[i][j] = a[i + new_dim][j];
      a22[i][j] = a[i + new_dim][j + new_dim];
      
      //b11[i][j] = b[i][j];
      b12[i][j] = b[i][j + new_dim];
      //b21[i][j] = b[i + new_dim][j];
      b22[i][j] = b[i + new_dim][j + new_dim];
    }
  }
  
  a11 = a;
  a21 = &a[new_dim]; 
  
  b11 = b;
  b21 = &b[new_dim];

  
  /*print_matrix(new_dim,a11);
  printf("\n");
  print_matrix(new_dim,a12);
  printf("\n");
  print_matrix(new_dim,a21);
  printf("\n");
  print_matrix(new_dim,a22);
  printf("\n");*/

  /*
    m1 = (a11+a22)*(b11+b22)
    m2 = (a21+a22)*b11
    m3 = a11*(b12-b22)
    m4 = a22(b21-b11)
    m5 = (a11+a12)*b22
    m6 = (a21-a11)*(b11+b12)
    m7 = (a12-a22)*(b21+b22)
  */

  sum(a11, a22, t1, new_dim);
  sum(b11, b22, t2, new_dim);
  strassen(t1, t2, m1, new_dim);
  
  sum(a21, a22, t1, new_dim);
  strassen(t1, b11, m2, new_dim);

  sub(b12, b22, t1, new_dim);
  strassen(a11, t1, m3, new_dim);

  sub(b21, b11, t1, new_dim);
  strassen(a22, t1, m4, new_dim);

  sum(a11, a12, t1, new_dim);
  strassen(t1, b22, m5, new_dim);

  sub(a21, a11, t1, new_dim);
  sum(b11, b12, t2, new_dim);
  strassen(t1, t2, m6, new_dim);

  sub(a12, a22, t1, new_dim);
  sum(b21, b22, t2, new_dim);
  strassen(t1, t2, m7, new_dim);

  /*
    c11 = m1+m4-m5+m7
    c12 = m3+m5
    c21 = m2+m4
    c22 = m1-m2+m3+m6
  */

  sum(m1, m4, t1, new_dim);
  sum(t1, m7, t2, new_dim);
  sub_m(t2, m5, c, new_dim, I);

  sum_m(m3, m5, c, new_dim, II);

  sum_m(m2, m4, c, new_dim, III);
  
  sum(m1, m3, t1, new_dim);
  sum(t1, m6, t2, new_dim);
  sub_m(t2, m2, c, new_dim, IV);

  //free

  //m_free(a11, new_dim);
  m_free(a12, new_dim);
  //m_free(a21, new_dim);
  m_free(a22, new_dim);
  //m_free(b11, new_dim);
  m_free(b12, new_dim);
  //m_free(b21, new_dim);
  m_free(b22, new_dim);

  m_free(m1, new_dim);
  m_free(m2, new_dim);
  m_free(m3, new_dim);
  m_free(m4, new_dim);
  m_free(m5, new_dim);
  m_free(m6, new_dim);
  m_free(m7, new_dim);
  m_free(t1, new_dim);
  m_free(t2, new_dim);
  
}
