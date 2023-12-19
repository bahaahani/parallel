#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <climits>

using namespace std;
int main()
{
    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int arraySize = 100;
    int *array = new int[arraySize];

    if (rank == 0)
    {
        // Initialize the array with random values
        for (int i = 0; i < arraySize; ++i)
        {
            array[i] = rand() % 100 + 10;
        }
    }

    int *localArray = new int[arraySize / size];
    MPI_Scatter(array, arraySize / size, MPI_INT, localArray, arraySize / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Find local minimum
    int localMin = INT_MAX;
    for (int i = 0; i < arraySize / size; ++i)
    {
        if (localArray[i] < localMin)
        {
            localMin = localArray[i];
        }
    }

    // Find global minimum
    int globalMin;
    MPI_Reduce(&localMin, &globalMin, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Print the array
        cout << "Array: ";
        for (int i = 0; i < arraySize; ++i)
        {
            cout << array[i] << " ";
        }
        cout << std::endl;

        // Print the result
        cout << "Minimum value: " << globalMin << endl;
    }

    delete[] array;
    delete[] localArray;

    MPI_Finalize();
    return 0;
}
