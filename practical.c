// Write an MPI program that calculates the integration of the following function using Trapezoidal rule:
// F(x) = 3x3 + 2 x2 - 7x
// For the interval 0 to 8 with number of intervals = 5040.
// Use Trapezoidal rule to calculate the integration.
// The intervals will be divided between the processes using block partitioning.
// Output the area.

#include <stdio.h>
#include <mpi.h>
#include <math.h>

double calculateFunction(double x)
{
    return 3 * pow(x, 3) + 2 * pow(x, 2) - 7 * x;
}

double calculateTrapezoidalArea(double start, double end, int intervals, double stepSize)
{
    double area = (calculateFunction(start) + calculateFunction(end)) / 2.0;
    for (int i = 1; i <= intervals - 1; i++)
    {
        double x_i = start + i * stepSize;
        area += calculateFunction(x_i);
    }
    area *= stepSize;
    return area;
}
int main(int argc, char *argv[])
{
    int processRank, totalProcesses, totalIntervals = 5040;
    double lowerLimit = 0.0, upperLimit = 8.0, stepSize, localStart, localEnd;
    double localArea, totalArea;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);
    stepSize = (upperLimit - lowerLimit) / totalIntervals;
    int localIntervals = totalIntervals / totalProcesses;
    localStart = lowerLimit + processRank * localIntervals * stepSize;
    localEnd = localStart + localIntervals * stepSize;
    localArea = calculateTrapezoidalArea(localStart, localEnd, localIntervals, stepSize);
    MPI_Reduce(&localArea, &totalArea, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (processRank == 0)
    {
        printf(" N = %d trapezoids of the integral from %f to %f = %f\n", totalIntervals, lowerLimit, upperLimit, totalArea);
    }
    MPI_Finalize();
    return 0;
}