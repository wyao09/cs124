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
void conventional( int **a, int **b, int **c, int d){
  if (d == 1){
    c[0][0] = a[0][0] * b[0][0];
    return;
  }
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

int main(int argc, char *argv[]){
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

  m_free(a, dim);
  m_free(b, dim);

  print_matrix(dim, c);

  m_free(c, dim);

  return 0;
}

/* Strassen's algorithm */

/*------------------------------------------------------------------------------*/
/* Sample code from wikipedia 
void strassen(double **a, double **b, double **c, int tam){
 
        // trivial case: when the matrice is 1 X 1:
        if(tam == 1){
                c[0][0] = a[0][0] * b[0][0];
                return;
        }
 
        // other cases are treated here:
        else{
                int novoTam = tam/2;
                double **a11, **a12, **a21, **a22;
                double **b11, **b12, **b21, **b22;
                double **c11, **c12, **c21, **c22;
                double **p1, **p2, **p3, **p4, **p5, **p6, **p7;
 
                // memory alocation:
                a11 = alocar_matriz_real(novoTam, -1);
                a12 = alocar_matriz_real(novoTam, -1);
                a21 = alocar_matriz_real(novoTam, -1);
                a22 = alocar_matriz_real(novoTam, -1);
 
                b11 = alocar_matriz_real(novoTam, -1);
                b12 = alocar_matriz_real(novoTam, -1);
                b21 = alocar_matriz_real(novoTam, -1);
                b22 = alocar_matriz_real(novoTam, -1);
 
                c11 = alocar_matriz_real(novoTam, -1);
                c12 = alocar_matriz_real(novoTam, -1);
                c21 = alocar_matriz_real(novoTam, -1);
                c22 = alocar_matriz_real(novoTam, -1);
 
                p1 = alocar_matriz_real(novoTam, -1);
                p2 = alocar_matriz_real(novoTam, -1);
                p3 = alocar_matriz_real(novoTam, -1);
                p4 = alocar_matriz_real(novoTam, -1);
                p5 = alocar_matriz_real(novoTam, -1);
                p6 = alocar_matriz_real(novoTam, -1);
                p7 = alocar_matriz_real(novoTam, -1);
 
                double **aResult = alocar_matriz_real(novoTam, -1);
                double **bResult = alocar_matriz_real(novoTam, -1);
 
                int i, j;
 
 
                //dividing the matrices in 4 sub-matrices:
            for (i = 0; i < novoTam; i++)
            {
                for (j = 0; j < novoTam; j++)
                {
                    a11[i][j] = a[i][j];
                    a12[i][j] = a[i][j + novoTam];
                    a21[i][j] = a[i + novoTam][j];
                    a22[i][j] = a[i + novoTam][j + novoTam];
 
                    b11[i][j] = b[i][j];
                    b12[i][j] = b[i][j + novoTam];
                    b21[i][j] = b[i + novoTam][j];
                    b22[i][j] = b[i + novoTam][j + novoTam];
                }
            }
 
                // Calculating p1 to p7:
 
                soma(a11, a22, aResult, novoTam); // a11 + a22
                soma(b11, b22, bResult, novoTam); // b11 + b22
                strassen(aResult, bResult, p1, novoTam); // p1 = (a11+a22) * (b11+b22)
 
                soma(a21, a22, aResult, novoTam); // a21 + a22
                strassen(aResult, b11, p2, novoTam); // p2 = (a21+a22) * (b11)
 
                subtrai(b12, b22, bResult, novoTam); // b12 - b22
                strassen(a11, bResult, p3, novoTam); // p3 = (a11) * (b12 - b22)
 
                subtrai(b21, b11, bResult, novoTam); // b21 - b11
                strassen(a22, bResult, p4, novoTam); // p4 = (a22) * (b21 - b11)
 
                soma(a11, a12, aResult, novoTam); // a11 + a12
                strassen(aResult, b22, p5, novoTam); // p5 = (a11+a12) * (b22)  
 
                subtrai(a21, a11, aResult, novoTam); // a21 - a11
                soma(b11, b12, bResult, novoTam); // b11 + b12
                strassen(aResult, bResult, p6, novoTam); // p6 = (a21-a11) * (b11+b12)
 
                subtrai(a12, a22, aResult, novoTam); // a12 - a22
                soma(b21, b22, bResult, novoTam); // b21 + b22
                strassen(aResult, bResult, p7, novoTam); // p7 = (a12-a22) * (b21+b22)
 
                // calculating c21, c21, c11 e c22:
 
                soma(p3, p5, c12, novoTam); // c12 = p3 + p5
                soma(p2, p4, c21, novoTam); // c21 = p2 + p4
 
                soma(p1, p4, aResult, novoTam); // p1 + p4
                soma(aResult, p7, bResult, novoTam); // p1 + p4 + p7
                subtrai(bResult, p5, c11, novoTam); // c11 = p1 + p4 - p5 + p7
 
                soma(p1, p3, aResult, novoTam); // p1 + p3
                soma(aResult, p6, bResult, novoTam); // p1 + p3 + p6
                subtrai(bResult, p2, c22, novoTam); // c22 = p1 + p3 - p2 + p6
 
 
                // Grouping the results obtained in a single matrice:
                 for (i = 0; i < novoTam ; i++)
            {
                for (j = 0 ; j < novoTam ; j++)
                {
                    c[i][j] = c11[i][j];
                    c[i][j + novoTam] = c12[i][j];
                    c[i + novoTam][j] = c21[i][j];
                    c[i + novoTam][j + novoTam] = c22[i][j];
                }
            }
 
 
 
                // unalocating de memória (free):
                a11 = liberar_matriz_real(a11, novoTam);
                a12 = liberar_matriz_real(a12, novoTam);
                a21 = liberar_matriz_real(a21, novoTam);
                a22 = liberar_matriz_real(a22, novoTam);
 
                b11 = liberar_matriz_real(b11, novoTam);
                b12 = liberar_matriz_real(b12, novoTam);
                b21 = liberar_matriz_real(b21, novoTam);
                b22 = liberar_matriz_real(b22, novoTam);
 
                c11 = liberar_matriz_real(c11, novoTam);
                c12 = liberar_matriz_real(c12, novoTam);
                c21 = liberar_matriz_real(c21, novoTam);
                c22 = liberar_matriz_real(c22, novoTam);
 
                p1 = liberar_matriz_real(p1, novoTam);
                p2 = liberar_matriz_real(p2, novoTam);
                p3 = liberar_matriz_real(p3, novoTam);
                p4 = liberar_matriz_real(p4, novoTam);
                p5 = liberar_matriz_real(p5, novoTam);
                p6 = liberar_matriz_real(p6, novoTam);
                p7 = liberar_matriz_real(p7, novoTam);
                aResult = liberar_matriz_real(aResult, novoTam);
                bResult = liberar_matriz_real(bResult, novoTam);
        } // end of else
 
} // end of strassen function
 
/*------------------------------------------------------------------------------
// function to sum two matrices
void soma(double **a, double **b, double **resultado, int tam){
 
        int i, j;
 
        for(i=0; i< tam; i++){
                for(j=0; j<tam; j++){
                        resultado[i][j] = a[i][j] + b[i][j];
                }
        }
}
 
/*------------------------------------------------------------------------------
// function to subtract two matrices
void subtrai(double **a, double **b, double **resultado, int tam){
 
        int i, j;
 
        for(i=0; i< tam; i++){
                for(j=0; j<tam; j++){
                        resultado[i][j] = a[i][j] - b[i][j];
                }
        }       
}
 
/*------------------------------------------------------------------------------
// This function alocates the matrice using malloc, and initializes it. If the variable randomico is passed
// as zero, it initializes the matrice with zero, if it's passed as 1, it initializes the matrice with randomic
// values. If it is passed with any other int value (like -1 for example) the matrice is initialized with no
// values in it. The variable tam defines the length of the matrice.
double **alocar_matriz_real (int tam, int randomico)
{
   int i, j, n = tam, m = tam;
   double **v, a;         // ponteiro para o vetor
 
  // alocates one vector of vectors (matrice)
   v = (double**)malloc(n*sizeof(double*));
 
   if (v == NULL) {
       printf ("** Error in matrice allocation: insuficient memory **");
       return (NULL);
       }
 
   // alocates each row of the matrice
   for(i=0;i<n;i++)
   {
      v[i] = (double*)malloc(m*sizeof(double));
 
                if (v[i] == NULL) {
               printf ("** Error: Insuficient memory **");
                           liberar_matriz_real(v, n);
               return (NULL);
       }
 
                // initializes the matrice with zeros
                if(randomico == 0){
                        for(j=0; j<m; j++)
                                v[i][j] = 0.0;
                }
 
                // initializes the matrice with randomic values between 0 and 10
                else{
                        if(randomico == 1){
                                for(j=0; j<m; j++){
                                        a = rand();
                                        v[i][j] = (a - (int)a) * 10;
                                }
                        }
                }
   }
 
   return (v);     // retorna o ponteiro para o vetor
}
 
/*------------------------------------------------------------------------------
// This function unalocates the matrice (frees memory)
double **liberar_matriz_real (double **v, int tam)
{ // inicio funçao
   int i;
   if (v == NULL) return (NULL);
 
        for(i=0;i<tam;i++){ 
                if(v[i]){ 
           free(v[i]); // frees a row of the matrice
                         v[i] = NULL;
                } 
        } 
 
   free(v);         // frees the pointer /
        v = NULL;
 
   return (NULL);   //returns a null pointer /
} // end of function
 
/*------------------------------------------------------------------------------*/
