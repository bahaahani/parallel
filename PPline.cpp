#define MPI_DATA_TAG 1
#define MPI_TERM_TAG 2
#include <iostream>
#include <mpi.h>
#define MAX 100

int main()
{
    int num;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
    {
        for (int i = 0; i < MAX; i++)
        {
            num = rand() % 101;
            if (num == 75)
            {
                MPI_Send(&num, 1, MPI_DOUBLE, rank + 1, MPI_TERM_TAG, MPI_COMM_WORLD);
                break;
            }
            else
                MPI_Send(&num, 1, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
        }
    }
    if (rank > 0)
    {
        while (true)
        {
            MPI_Recv(&num, 1, MPI_DOUBLE, rank - 1, MPI_DATA_TAG, MPI_COMM_WORLD, MPI_ANY_STATUS);
            if (status.MPI_TAG == MPI_TERM_TAG)
            {
                if (rank != size - 1)
                    MPI_Send(&num, 1, MPI_DOUBLE, rank + 1, MPI_TERM_TAG, MPI_COMM_WORLD);
                break;
            }
            if (rank == 1)
            {
                num -= 4;
            }
            if (rank == 2)
            {
                num /= 2;
            }
            if (rank == 3)
            {
                num += 13;
            }
            if (rank < size - 1)
            {
                MPI_Send(&num, 1, MPI_DOUBLE, rank + 1, _MPI_DATA_TAG, MPI_COMM_WORLD);
            }
            else
                cout << num << endl;
        }
    }
    MPI_Finalize();
    return 0;
}