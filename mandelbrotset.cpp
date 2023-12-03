#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int mandelbrotCalculate(double real, double imag, int max_iter)
{
    double zReal = real;
    double zImag = imag;
    for (int n = 0; n < max_iter; ++n)
    {
        double r2 = zReal * zReal;
        double i2 = zImag * zImag;
        if (r2 + i2 > 4.0)
            return n;
        zImag = 2.0 * zReal * zImag + imag;
        zReal = r2 - i2 + real;
    }
    return max_iter;
}

int main(int argc, char *argv[])
{
    int rank, size, i, j, n, max_iter = 1000;
    double minX = -2.0, maxX = 1.0, minY = -1.5, maxY = 1.5;
    int width = 800, height = 600;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int rows_per_process = height / size;
    int remainder = height % size;
    int start_row, end_row;

    // Distribute extra rows among the first few processes
    if (rank < remainder)
    {
        start_row = rank * (rows_per_process + 1);
        end_row = start_row + rows_per_process;
    }
    else
    {
        start_row = rank * rows_per_process + remainder;
        end_row = start_row + (rows_per_process - 1);
    }

    for (i = start_row; i <= end_row; ++i)
    {
        for (j = 0; j < width; ++j)
        {
            double x = minX + (maxX - minX) * j / (width - 1);
            double y = minY + (maxY - minY) * i / (height - 1);
            int iter = mandelbrotCalculate(x, y, max_iter);
            // Do something with iter, for example, print it
            printf("Process %d, Pixel [%d,%d], Iteration: %d\n", rank, i, j, iter);
        }
    }

    MPI_Finalize();
    return 0;
}
