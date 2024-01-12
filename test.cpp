#include <iostream>
#include <mpi.h>
// if its odd. Then output the result
#define MAX 100
#define MPI_DATA_TAG 0
#define MPI_TERM_TAG 1
using namespace std;
int main()
{
    MPI_INIT(NULL, NULL);
    int size, rank, num;
    int A[MAX];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Do a pipeline code that will terminate once you finish sending 100 elements,
    if (rank == 0)
    {
        // generate 100 elements
        for (int i = 0; i < MAX; i++)
        {
            num = rand() % 101;
            A[i] = num;
            if (num == 50) // or the number generated equals 50.
                MPI_Send(&A, MAX, MPI_INT, MPI_TERM_TAG, rank + 1, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Status status;
            MPI_Send(&A, MAX, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
        }
    }
    // Processors1 will add 3,
    if (rank > 0)
    {
        while (true)
        {
            MPI_Recv(&num, 1, MPI_INT, rank - 1, MPI_COMM_WORLD, &status);
            if (MPI_ANY_TAG == MPI_DATA_TAG)
            {
                if (rank == 1)
                {
                    num += 3;
                    MPI_Send(&num, 1, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
                }
                // Processor2 will multiply by 4
                if (rank == 2)
                {
                    MPI_Recv(&num, 1, MPI_INT, rank - 1, MPI_DATA_TAG, MPI_COMM_WORLD, &status);
                    num *= 4;
                    MPI_Send(&num, 1, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
                } // Processor3 will subtract 3 if its even, or it will add 1
                if (rank == 3)
                {
                    MPI_Recv(&num, 1, MPI_INT, rank - 1, MPI_DATA_TAG, MPI_COMM_WORLD, &status);
                    num = (num % 2 == 0) ? num - 3 : num + 1;
                    MPI_Send(&num, 1, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
                }
            }
            else
            {
                cout << num << endl;
                if (rank != size - 1)
                    MPI_Send(&num, 1, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
                else
                {
                    if (rank != size - 1)
                    {
                        MPI_Send(&num, 1, MPI_INT, rank + 1, MPI_TERM_TAG, MPI_COMM_WORLD);
                        break;
                    }
                }
            }
        }
    }
    MPI_Finalize();
    return 0;
}

// int block = max/size;
// int low= rank * block;
// int high (rank== size-1) ? N : low+block;
// //generate 100 elements
// for(int i=low;i<high;i++){
//     if(A[i] %2 ==0){
//         loca_sum += A[i];
//     }
// }