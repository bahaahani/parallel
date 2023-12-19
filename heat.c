#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 400
#define TEMP_EDGE 100
#define TEMP_INIT 40
#define MAX_CYCLES 1000

int main() {
    double (*plate)[SIZE] = malloc( sizeof(double[SIZE][SIZE]) );
    double (*new_plate)[SIZE] = malloc( sizeof(double[SIZE][SIZE]) );
    int i, j, cycle;

    clock_t begin = clock();

    // Initialize the plate
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (i == 0 || i == SIZE-1 || j == 0 || j == SIZE-1)
                plate[i][j] = TEMP_EDGE;
            else
                plate[i][j] = TEMP_INIT;
        }
    }

    // Simulation cycles
    for (cycle = 0; cycle < MAX_CYCLES; cycle++) {
        #pragma omp parallel for private(j) schedule(dynamic)
        for (i = 1; i < SIZE - 1; i++) {
            for (j = 1; j < SIZE - 1; j++) {
                new_plate[i][j] = (plate[i-1][j] + plate[i+1][j] + plate[i][j-1] + plate[i][j+1]) / 4.0;
            }
        }

        // Update the plate for the next iteration
        double (*tmp)[SIZE] = plate;
        plate = new_plate;
        new_plate = tmp;
    }

    // Find the maximum and minimum temperatures
    double max_temp = 0.0;
    double min_temp = 100.0;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (plate[i][j] > max_temp) max_temp = plate[i][j];
            if (plate[i][j] < min_temp) min_temp = plate[i][j];
        }
    }

    clock_t end = clock();

    printf("Max Temperature: %f\n", max_temp);
    printf("Min Temperature: %f\n", min_temp);
    printf("\nElapsed: %f seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);

    free(plate);
    free(new_plate);
    return 0;
}

