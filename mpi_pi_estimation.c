
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size, i;
    int local_count = 0, total_count = 0;
    double x, y, distance_squared;
    int tosses_per_process;
    double PI_Estimation;
    int number_of_tosses = 100000;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Seed the random number generator
    srand(time(NULL) + rank);

    tosses_per_process = number_of_tosses / size;

    // Each process performs its tosses
    for (i = 0; i < tosses_per_process; i++) {
        x = (rand() / (double)RAND_MAX) * 2 - 1;
        y = (rand() / (double)RAND_MAX) * 2 - 1;
        distance_squared = x * x + y * y;
        if (distance_squared <= 1) {
            local_count++;
        }
    }

    // Gather all the local counts to the master process
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Master process calculates the PI estimation
    if (rank == 0) {
        PI_Estimation = 4.0 * total_count / (double)number_of_tosses;
        printf("PI Estimation = %f\n", PI_Estimation);
    }

    MPI_Finalize();
    return 0;
}
