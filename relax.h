#ifndef RELAX_H
#define RELAX_H

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

#define AVG(N, S, E, W) (N + S + E + W)/4.0

typedef struct {
  int world_size;
  int dimension;
  float precision;
  num* current;
  num* next;
} global;

typedef struct {
  int to;
  int from;
} range;

typedef struct {
  global* g;
  range r;
} params;

global* create_globals(int dimension, num precision, int threads);
params* create_params(global* g, int from, int to);
num* relax(int dimension, num precision, int threads);

#endif // RELAX_H
