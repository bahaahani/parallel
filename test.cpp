#include <iostream>
#include <mpi.h>
#include <cstdlib> // for rand()

#define MAX 100
#define MPI_DATA_TAG 0
#define MPI_TERM_TAG 1

using namespace std;

int main()
{
    MPI_Init(NULL, NULL); // Corrected: MPI_INIT should be MPI_Init
    int size, rank;
    int A[MAX];
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        // Rank 0: generate numbers and send them to the next process
        for (int i = 0; i < MAX; i++)
        {
            A[i] = rand() % 101;
        }
        // Send the array to the next process
        MPI_Send(A, MAX, MPI_INT, 1, MPI_DATA_TAG, MPI_COMM_WORLD);
        // Send a termination signal
        MPI_Send(NULL, 0, MPI_INT, 1, MPI_TERM_TAG, MPI_COMM_WORLD);
    }
    else
    {
        bool running = true;
        while (running)
        {
            MPI_Recv(A, MAX, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == MPI_DATA_TAG)
            {
                // Process the data
                if (rank == 1)
                {
                    for (int i = 0; i < MAX; i++)
                    {
                        A[i] += 3; // Processor1 will add 3
                    }
                }
                else if (rank == 2)
                {
                    for (int i = 0; i < MAX; i++)
                    {
                        A[i] *= 4; // Processor2 will multiply by 4
                    }
                }
                else if (rank == 3)
                {
                    for (int i = 0; i < MAX; i++)
                    {
                        // Processor3 will subtract 3 if it's even, or it will add 1
                        A[i] = (A[i] % 2 == 0) ? A[i] - 3 : A[i] + 1;
                    }
                }
                // Pass the array to the next process, unless it's the last rank
                if (rank < size - 1)
                {
                    MPI_Send(A, MAX, MPI_INT, rank + 1, MPI_DATA_TAG, MPI_COMM_WORLD);
                }
            }
            else if (status.MPI_TAG == MPI_TERM_TAG)
            {
                // If a termination signal is received, pass it on and stop the loop
                if (rank < size - 1)
                {
                    MPI_Send(NULL, 0, MPI_INT, rank + 1, MPI_TERM_TAG, MPI_COMM_WORLD);
                }
                running = false;
            }
        }
    }

    // If it's the last rank, output the result
    if (rank == size - 1)
    {
        for (int i = 0; i < MAX; i++)
        {
            if (A[i] % 2 != 0)
            { // if it's odd, then output the result
                cout << "A[" << i << "] = " << A[i] << endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
