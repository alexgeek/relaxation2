#include "relax.h"

// initialises all globals in a struct that each thread has a pointer to
global* create_globals(int dimension, float precision, int world_size)
{
  global* g = (global*)malloc(sizeof(global));
  g->world_size = world_size;
  g->dimension = dimension;
  g->precision = precision;
  g->current = matrix_alloc_square(dimension);
  g->next = matrix_alloc_square(dimension);
  matrix_init_edge(g->current, dimension, 1, 0);
  matrix_init_edge(g->next, dimension, 1, 0);
  return g;
}

num* relax(int dimension, num precision, int world_size, int rank);
  global* g = create_globals(dimension, precision, world_size);

  // MPI_barrier

  // while (precision not met)

    // get range for given rank

    // do averages, set flag to false is precision not met

    // send data +/- 1 rank

  // end while and send results to master



  return g->next;
}
