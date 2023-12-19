#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int done = 0, n, myid, numprocs, i;
    double PI = 3.1415926535897932;
    double mypi, pi, h, sum, x;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0)
    {
        printf("Enter number of intervals: ");
        scanf("%d", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    h = 1.0 / (double)n; // size of interval
    sum = 0.0;

    for (i = myid + 1; i <= n; i += numprocs)
    {
        x = h * ((double)i - 0.5);
        sum += h * 4.0 / (1.0 + x * x);
    }

    mypi = sum;

    if (myid != 0)
        MPI_Send(&mypi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    else
    {
        pi = mypi;
        for (i = 1; i < numprocs; i++)
        {
            MPI_Recv(&mypi, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            pi += mypi;
        }
        printf("Approx PI is %16f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
