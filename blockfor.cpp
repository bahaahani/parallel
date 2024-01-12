// write an MPI program that will find the average
// of a number in a n array of size max
// using block data partition strategy:
#include <iostream>
#include <mpi.h>
#define max 100
int main()
{
    int rank, result, size, sum, block;
    float avg = 0;
    int A;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Bcast(&A, MAX, MPI_INT, 0, MPI_COMM_WORLD);
    int begin, end;
    block = MAX / size;
    begin = block * rank;
    end = (rank != size - 1) ? begin + size : n;
    for (int i = begin; i < end; i++)
    {
        sum += A[i];
    }

    // cyclic data distribution
    for (int i = rank; i < Max; i += size)
    {
        sum += A[i];
    }
    MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    float avg = &result / MAX;
    cout << avg << endl;
    MPI_Finalize();
    return 0;
}