#include "mpi.h"
#include <stdio.h>
#include "matrix.h"
#include "relax.h"
#include <unistd.h>

#define EXIT_SUCCESS 0

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  int dimension = 10;
  num precision = 0.01;
  int verbose = 0;

  int opt;
  while ((opt = getopt (argc, argv, "vn:p:")) != -1)
  {
    switch(opt) {
      case 'n':
        if(sscanf(optarg, "%i", &dimension) != 1)
          fprintf(stderr, "Invalid dimension, using default (%i).\n", dimension);
        if(dimension < 5) {
          fprintf(stderr, "Use a mininum dimension of 5.\n");
        }
        break;
      case 'p':
        if(sscanf(optarg, "%lf", &precision) != 1)
          fprintf(stderr, "Invalid precision, using default (%f).\n", precision);
        if(precision < 0) {
          fprintf(stderr, "Precision cannot be negative.\n");
          precision = 0.01;
        }
        break;
      case 'v':
        verbose = 1;
    }
  }

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
  printf("Processor %s, rank %d out of %d processors\n",
         processor_name, world_rank, world_size);

  num* result = relax(dimension, precision, world_size, world_rank);

  if(world_rank == MASTER && verbose) {
    printf("\nFinal Results (n=%d, p=%g):\n", dimension, precision);
    matrix_print(result, dimension);
  }

  // Finalize the MPI environment.
  MPI_Finalize();

  return EXIT_SUCCESS;
}
