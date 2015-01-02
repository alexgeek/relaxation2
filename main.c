#include "mpi.h"
#include <stdio.h>
#include "matrix.h"

#define MASTER 0
#define EXIT_SUCCESS 0

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // Print off a hello world message
  printf("Hello world from processor %s, rank %d out of %d processors\n",
         processor_name, world_rank, world_size);

  int out, in = 10;

 	MPI_Allreduce(&in, &out, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  printf("out = %d", out);

  if(world_rank == MASTER) {
    printf("I am the master.\n");
    int size = 5;
    num* a = matrix_alloc_square(size);
    num* b = matrix_alloc_square(size);
    matrix_init_to(a, size, 0);
    matrix_init_to(b, size, 1);
    printf("a = %f, b = %f", a[1], b[1]);
    swap_pointers(&a, &b);
    printf("a = %f, b = %f", a[1], b[1]);
  }

  // Finalize the MPI environment.
  MPI_Finalize();

  return EXIT_SUCCESS;
}
