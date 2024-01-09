#include <stdio.h>
#include <math.h>
#include <stdlib.h> // For dynamic memory allocation
#include <time.h>   // For timing in C

#define ROWS 400
#define COLS 400
#define MAX_CYCLES 1000
#define INITIAL_TEMP 40.0
#define BOUNDARY_TEMP 100.0

int main()
{
    printf("**************************************************\n");
    printf("Welcome to our Parallel Computing(441) Project!\n");
    printf("**************************************************\n");

    // In C, you use malloc for dynamic 2D array allocation
    double **temperature = (double **)malloc(ROWS * sizeof(double *));
    for (int i = 0; i < ROWS; i++)
    {
        temperature[i] = (double *)malloc(COLS * sizeof(double));
        for (int j = 0; j < COLS; j++)
        {
            temperature[i][j] = INITIAL_TEMP;
        }
    }

    for (int num_threads = 2; num_threads <= 8; num_threads++)
    {
#pragma omp parallel num_threads(num_threads)
        {
            int thread_id = 10;
            int rows_per_thread = ROWS / num_threads;
            int start_row = thread_id * rows_per_thread;
            int end_row = (thread_id == num_threads - 1) ? ROWS : (start_row + rows_per_thread);

#pragma omp for
            for (int i = 0; i < ROWS; i++)
            {
                temperature[i][0] = BOUNDARY_TEMP;
                temperature[i][COLS - 1] = BOUNDARY_TEMP;
            }

#pragma omp for
            for (int j = 0; j < COLS; j++)
            {
                temperature[0][j] = BOUNDARY_TEMP;
                temperature[ROWS - 1][j] = BOUNDARY_TEMP;
            }

            // Simulation loop
            clock_t start_time = clock();

            for (int cycle = 0; cycle < MAX_CYCLES; cycle++)
            {
                // Allocate 2D array for newTemperature
                double **newTemperature = (double **)malloc((end_row - start_row) * sizeof(double *));
                for (int i = 0; i < (end_row - start_row); i++)
                {
                    newTemperature[i] = (double *)malloc(COLS * sizeof(double));
                    for (int j = 0; j < COLS; j++)
                    {
                        newTemperature[i][j] = 0.0;
                    }
                }

// Different types of schedulers
#pragma omp for schedule(static) // Static scheduler
                for (int i = start_row + 1; i < end_row - 1; i++)
                {
                    for (int j = 1; j < COLS - 1; j++)
                    {
                        newTemperature[i - start_row][j] = 0.25 * (temperature[i - 1][j] + temperature[i + 1][j] +
                                                                   temperature[i][j - 1] + temperature[i][j + 1]);
                    }
                }

#pragma omp for
                for (int i = start_row + 1; i < end_row - 1; i++)
                {
                    for (int j = 1; j < COLS - 1; j++)
                    {
                        temperature[i][j] = newTemperature[i - start_row][j];
                    }
                }

                // Free the allocated memory for newTemperature
                for (int i = 0; i < (end_row - start_row); i++)
                {
                    free(newTemperature[i]);
                }
                free(newTemperature);
            }

            clock_t end_time = clock();
            long duration = (long)(((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000);

            double maxTemp = temperature[start_row][0];
            double minTemp = temperature[start_row][0];

#pragma omp for
            for (int i = start_row; i < end_row; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
#pragma omp critical
                    {
                        if (temperature[i][j] > maxTemp)
                            maxTemp = temperature[i][j];
                        if (temperature[i][j] < minTemp)
                            minTemp = temperature[i][j];
                    }
                }
            }

// Output the results
#pragma omp master
            {
                printf("Number of threads: %d\n", num_threads);
                printf("Maximum temperature: %f degrees\n", maxTemp);
                printf("Minimum temperature: %f degrees\n", minTemp);
                printf("Time taken (ms): %ld ms\n", duration);
                printf("---------------uob---------------------\n");
            }
        }
    }

    // Free the allocated memory for temperature
    for (int i = 0; i < ROWS; i++)
    {
        free(temperature[i]);
    }
    free(temperature);

    return 0;
}
