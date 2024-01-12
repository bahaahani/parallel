#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int MAX_ELEMENTS = 100;
    int data;

    if (rank == 0)
    {
        // Processor1: Generate numbers and add 3
        for (int i = 0; i < MAX_ELEMENTS; ++i)
        {
            data = i + 3;
            MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            if (data == 50)
                break;
        }
    }
    else if (rank == 1)
    {
        // Processor2: Receive, multiply by 4, and send to next processor
        for (int i = 0; i < MAX_ELEMENTS; ++i)
        {
            MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (data == 50)
            {
                MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                break;
            }
            data *= 4;
            MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }
    else if (rank == 2)
    {
        // Processor3: Receive, add/subtract, and print result
        while (true)
        {
            MPI_Recv(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (data == 50)
                break;
            if (data % 2 == 0)
            {
                data -= 3;
            }
            else
            {
                data += 1;
            }
            std::cout << "Result: " << data << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
