/*
 * strassen.c - 
 *
 * usage: ./strassen 0 <dimension> <inputﬁle>
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


int main(int argc, char **argv){
  if (argc != 4){
    printf("usage: ./strassen 0 <dimension> <inputﬁle>\n");
    return 1;
  }   

  // commandline input
  int dim = atoi(argv[2]);

  int **a = m_malloc(dim);
  int **b = m_malloc(dim);
  int **c = m_malloc(dim);

  rand_matrix(dim, a);
  print_matrix(dim, a);
  printf("\n");
  rand_matrix(dim, b);
  print_matrix(dim, b);

  printf("\n");
  conventional(a, b, c, dim);
  //printf("strassens!\n");
  //strassen(a, b, c, dim);

  m_free(a, dim);
  m_free(b, dim);

  print_matrix(dim, c);

  m_free(c, dim);

  return 0;
}


/* generate random matrix */
// need to add argument that determines type of random matrix
void rand_matrix(int n, int **m){
  srand(time(NULL)); // need to us POSIX time?
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

/* conventional matrix multiplication */
// for now assume square matrices (can we assume this ??)
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

void sub(int **a, int **b, int **c, int d){
  int i,j;
  for (i=0; i<d; i++)
    for (j=0; j<d; j++)
      c[i][j] = a[i][j] - b[i][j];
}

void strassen(int **a, int **b, int **c, int d){
  if (d == 1){
    c[0][0] = a[0][0] * b[0][0];
    return;
  }

  int i, j;
  int new_dim = d/2; //this assume that n is even?

  int **a11, **a12, **a21, **a22;
  int **b11, **b12, **b21, **b22;
  int **c11, **c12, **c21, **c22;
  int **m1, **m2, **m3, **m4, **m5, **m6, **m7;
  int **t1, **t2;

  a11 = m_malloc(new_dim);
  a12 = m_malloc(new_dim);
  a21 = m_malloc(new_dim);
  a22 = m_malloc(new_dim);
  b11 = m_malloc(new_dim);
  b12 = m_malloc(new_dim);
  b21 = m_malloc(new_dim);
  b22 = m_malloc(new_dim);
  c11 = m_malloc(new_dim);
  c12 = m_malloc(new_dim);
  c21 = m_malloc(new_dim);
  c22 = m_malloc(new_dim);
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
  // is it necessary to actually copy each entry?
  for (i=0; i<new_dim; i++){
    for (j=0; j<new_dim; j++){
      a11[i][j] = a[i][j];
      a12[i][j] = a[i][j + new_dim];
      a21[i][j] = a[i + new_dim][j];
      a22[i][j] = a[i + new_dim][j + new_dim];
      
      b11[i][j] = b[i][j];
      b12[i][j] = b[i][j + new_dim];
      b21[i][j] = b[i + new_dim][j];
      b22[i][j] = b[i + new_dim][j + new_dim];
    }
  }

  // base case
  // if (new_dem < x)

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

  // do we really need the sub c? cant we just place them in c directly?
  sum(m1, m4, t1, new_dim);
  sum(t1, m7, t2, new_dim);
  sub(t2, m5, c11, new_dim);

  sum(m3, m5, c12, new_dim);

  sum(m2, m4, c21, new_dim);
  
  sum(m1, m3, t1, new_dim);
  sum(t1, m6, t2, new_dim);
  sub(t2, m2, c22, new_dim);

  // consolidate
  for (i = 0; i < new_dim ; i++){
    for (j = 0 ; j < new_dim ; j++){
      c[i][j] = c11[i][j];
      c[i][j + new_dim] = c12[i][j];
      c[i + new_dim][j] = c21[i][j];
      c[i + new_dim][j + new_dim] = c22[i][j];
    }
  }

  //free
  m_free(a11, new_dim);
  m_free(a12, new_dim);
  m_free(a21, new_dim);
  m_free(a22, new_dim);
  m_free(b11, new_dim);
  m_free(b12, new_dim);
  m_free(b21, new_dim);
  m_free(b22, new_dim);
  m_free(c11, new_dim);
  m_free(c12, new_dim);
  m_free(c21, new_dim);
  m_free(c22, new_dim);
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
