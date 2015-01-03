#include "relax.h"

// initialises all globals in a struct that each thread has a pointer to
global* create_globals(int dimension, float precision, int world_size) {
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

range make_range(int from, int to) {
  range r;
  r.from = from;
  r.to = to;
  r.size = r.to - r.from;
  return r;
}

range get_range(global* g, int rank) {
  range r;
  int part_size = (g->dimension - 2) / g->world_size;
  r.from = rank * part_size + 1; // start at 1 to avoid edges
  // special case for last
  r.to = rank < g->world_size - 1 ? r.from + part_size : g->dimension - 1;
  r.size = r.to - r.from;
  return r;
}

// relaxes one row
int relax_row(num* current, num* next, int row, int length, num precision)
{
  int finished = 1; // flag as relaxed by default
  int col;
  // loop through row ignoring edges
  for(col = 1; col < length-1; col++)
  {
    // 2d indices -> 1d index
    int i = row*length + col;
    // the relaxation operation
    next[i] = AVG(current[i-1], current[i+1], current[i+length], current[i-length]);
    // if any do not meet precision, flag as not relaxed
    if (!CHECK(next[i], current[i], precision))
      finished = 0;
  }
  // 1 if row is relaxed, 0 if any are not relaxed yet
  return finished;
}

// matptr starts at position in the array to begin sending ( ending  with + size)
int send_row(num* matptr, int size, int to_rank, int tag) {
  MPI_Send(matptr, size, MPI_DOUBLE, to_rank, tag, MPI_COMM_WORLD);
  return 0; // TODO error check;
}

// matptr starts at position in array to start receiving from (ending with + size)
int recv_row(num* matptr, int size, int from_rank, int tag) {
  MPI_Recv(matptr, size, MPI_DOUBLE, from_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  return 0;
}

num* relax(int dimension, num precision, int world_size, int rank) {
  global* g = create_globals(dimension, precision, world_size);
  range r = get_range(g, rank);
  //printf("%d will work on [%d, %d)\n", rank, r.from, r.to);

  // ranges to receive data into
  range up_range, down_range;
  //
  if(rank == MASTER) {
    if(world_size > 1)
      up_range = get_range(g, rank + 1);
    else
    {
      up_range.from = 0;
      up_range.to = 0;
    }
    down_range.from = 0;
    down_range.to = 0;
  }
  else if(rank == world_size - 1) {
    down_range = get_range(g, rank - 1);
    up_range.from = 0;
    up_range.to = 0;
  } else {
    up_range = get_range(g, rank + 1);
    down_range = get_range(g, rank - 1);
  }

  int finished;
  int iter = 0;
  do {
    finished = 1; // assume finished
    int row;
    iter++;
    for (row = r.from; row < r.to; row++) {
      //printf("%d working on %d (iteraton %d)\n", rank, row, iter);
      // if row did not relax, set flag to not finished
      if(!relax_row(g->current, g->next, row, g->dimension, g->precision))
        finished = 0;
    }

    // all send finished flag, get minimum
    // so if all finished, minimum = 1
    // if any did not finish, minimum = 0
    MPI_Allreduce(&finished, &finished, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    // not finished
    if(finished == 0) {

      // send data +- 1 rank
      if(up_range.from != 0) {
        // send bottom row to rank + 1
        send_row(&(g->next[(r.to-1)*g->dimension]), g->dimension, rank+1, TAG_NEIGHBOUR);
      }
      if(down_range.from != 0) {
        // send top row to rank - 1
        send_row(&(g->next[(r.from)*g->dimension]), g->dimension, rank-1, TAG_NEIGHBOUR);
      }

      // recv data +- 1 rank
      if(up_range.from != 0) {
        // recv bottom row + 1 from up
        recv_row(&(g->next[r.to * g->dimension]), g->dimension, rank+1, TAG_NEIGHBOUR);
      }
      if (down_range.from != 0) {
        // recv top row - 1 from down
        recv_row(&(g->next[(r.from-1) * g->dimension]), g->dimension, rank-1, TAG_NEIGHBOUR);
      }

      // quicker than memcpy
      swap_pointers(&(g->current), &(g->next));
    }


    //if(iter == 2) break;
    //matrix_print(g->next, g->dimension);

  } while(!finished);

  // slaves all send their part to master
  if(rank != MASTER) {
    send_row(&(g->next[r.from*g->dimension]), r.size*g->dimension, MASTER, TAG_FINISH);
  } else {
    // MASTER receives chunks of rows from slaves
    int i;
    for(i = MASTER+1; i < world_size; i++) {
      range slave_range = get_range(g, i);
      recv_row(&(g->next[slave_range.from * g->dimension]), slave_range.size*g->dimension, i, TAG_FINISH);
    }
  }

  // MPI_barrier

  // while (precision not met)

    // get range for given rank

    // do averages, set flag to false is precision not met

    // send data +/- 1 rank

  // end while and send results to master

  //MPI_Barrier(MPI_COMM_WORLD);

  return g->next;
}
