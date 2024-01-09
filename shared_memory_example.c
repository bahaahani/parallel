#include <iostream>
#include <malloc.h>
#include <omp.h>
using namespace std;

#define SIZE 400
#define EDGE_TEMP 100
#define INTERNAL_INITIAL_TEMP 40
#define CYCLES 1000
#define THREADS 8

int main()
{
    double max_temp = INTERNAL_INITIAL_TEMP,
           min_temp = EDGE_TEMP;

    double **plate = new double *[SIZE];
    double **temp_plate = new double *[SIZE];

    for (int i = 0; i < SIZE; ++i)
    {
        plate[i] = new double[SIZE];
        temp_plate[i] = new double[SIZE];
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (i == 0 || i == SIZE - 1 || j == 0 || j == SIZE - 1)
                plate[i][j] = EDGE_TEMP;
            else
                plate[i][j] = INTERNAL_INITIAL_TEMP;
        }
    }

    double start = omp_get_wtime();

    for (int iter = 0; iter < CYCLES; iter++)
    {
#pragma omp parallel for num_threads(THREADS)
        for (int i = 1; i < SIZE - 1; i++)
        {
            for (int j = 1; j < SIZE - 1; j++)
            {
                temp_plate[i][j] = 0.25 * (plate[i - 1][j] + plate[i + 1][j] + plate[i][j - 1] + plate[i][j + 1]);
            }
        }
        for (int i = 1; i < SIZE - 1; i++)
        {
            for (int j = 1; j < SIZE - 1; j++)
            {
                plate[i][j] = temp_plate[i][j];
            }
        }
    }

    double end = omp_get_wtime();

    for (int i = 1; i < SIZE - 1; i++)
    {
        for (int j = 1; j < SIZE - 1; j++)
        {
            if (plate[i][j] > max_temp)
                max_temp = plate[i][j];
            if (plate[i][j] < min_temp)
                min_temp = plate[i][j];
        }
    }

    cout << "Number of threads = " << THREADS << "\n";
    cout << "The minimum temperature in the plate is " << min_temp << "\n";
    cout << "The maximum temperature in the plate is " << max_temp << "\n";
    cout << "Time Elapsed = " << end - start << "\n";

    for (int i = 0; i < SIZE; ++i)
    {
        delete[] plate[i];
        delete[] temp_plate[i];
    }
    delete[] plate;
    delete[] temp_plate;

    return 0;
}
