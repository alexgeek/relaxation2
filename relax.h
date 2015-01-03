#ifndef RELAX_H
#define RELAX_H

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"

#define MASTER 0
#define AVG(N, S, E, W) (N + S + E + W)/4.0
#define CHECK(NEW, OLD, P) fabs(NEW-OLD) < P

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
  int size;
} range;

typedef struct {
  global* g;
  range r;
} params;

global* create_globals(int dimension, float precision, int world_size);
range make_range(int from, int to);
range get_range(global* g, int rank);
int send_row(num* matptr, int size, int to_rank);
int recv_row(num* matptr, int size, int from_rank);
num* relax(int dimension, num precision, int world_size, int rank);

#endif // RELAX_H
