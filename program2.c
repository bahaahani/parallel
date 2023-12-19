#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2)
    {
        fprintf(stderr, "World size must be two for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int ARRAY_SIZE = 1000;
    int array[ARRAY_SIZE]; // Or char, double as needed
    double start_time, end_time, time_taken;

    if (world_rank == 0)
    {
        // P0: Send each array element
        start_time = MPI_Wtime();
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            MPI_Send(&array[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        end_time = MPI_Wtime();
        time_taken = end_time - start_time;
        printf("P0 Sending Time: %f\n", time_taken);

        // P0: Receive time from P1
        MPI_Recv(&time_taken, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P1 Receiving Time: %f\n", time_taken);
    }
    else if (world_rank == 1)
    {
        // P1: Receive each array element
        start_time = MPI_Wtime();
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            MPI_Recv(&array[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        end_time = MPI_Wtime();
        time_taken = end_time - start_time;
        printf("P1 Receiving Time: %f\n", time_taken);

        // P1: Send time to P0
        MPI_Send(&time_taken, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}