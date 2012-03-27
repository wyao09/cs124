/*
 * strassen.h -
 *
 * assignment: cs124 pa2
 *
 * authors: Aidan Daly <aidan.daly@college.harvard.edu>
 *          Willie Yao <wyao13@college.harvard.edu>
 */

void rand_matrix(int n, int **m);
void print_matrix(int n, int **m);
void conventional(int **a, int **b, int **c, int d);
void transpose(int **m, int d);
void transpose_conv(int **a, int **b, int **c, int d);
int **m_malloc(int dim);
void m_free(int **m, int dim);

void sum(int **a, int **b, int **c, int d);
void sum_m(int **a, int **b, int **c, int new_d, int mode);
void sub(int **a, int **b, int **c, int d);
void sub_m(int **a, int **b, int **c, int new_d, int mode);
void strassen(int **a, int **b, int **c, int d);
