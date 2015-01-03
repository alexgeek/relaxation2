#include "matrix.h"

num* matrix_alloc(int width, int height)
{
    num* mat = (num*)malloc(width*height*sizeof(num));
    return mat;
}

num* matrix_alloc_square(int n) {
    return matrix_alloc(n, n);
}

void matrix_init_to(num* mat, int n, num to) {
    int i;
    for (i = 0; i < n; i++)
      mat[i] = to;
}

void matrix_init_edge(num* mat, int n, num edge_val, num inner_val) {
    int i, j;
    for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
        mat[i*n + j] = (i == 0 || j == 0 )//|| i == n-1 || j == n-1)
          ? edge_val : inner_val;
}

void matrix_print(const num* mat, int n) {
  int i, j;
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      printf("%.4f ", mat[n*i+j]);
    }
    printf("\n");
  }
  printf("\n");
}

void swap_pointers(num** a, num** b) {
  num* tmp = *a;
  *a = *b;
  *b = tmp;
}
