#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>

typedef double num;

num* matrix_alloc(int width, int height);
num* matrix_alloc_square(int n);
void matrix_init_to(num* mat, int n, num to);
void matrix_init_edge(num* mat, int n, num edge_val, num inner_val);
void matrix_print(const num* mat, int n);
void swap_pointers(num** a, num** b);

#endif // MATRIX_H
