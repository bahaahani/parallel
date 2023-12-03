#include <stdio.h>

// Function to be integrated
double f(double x)
{
    // Example function: f(x) = x^2
    return x * x;
}

// Trapezoidal Rule Function
double trapezoidalRule(double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.0;

// Parallelize the loop with OpenMP
#pragma omp parallel for default(none) private(i) shared(a, b, n, h, sum) reduction(+ : sum) schedule(guided)
    for (int i = 0; i < n; i++)
    {
        double x_i = a + i * h;
        double x_i1 = a + (i + 1) * h;
        sum += 0.5 * (f(x_i) + f(x_i1)) * h;
    }

    return sum;
}

int main()
{
    double lowerLimit = 0;    // Lower limit of integration
    double upperLimit = 10;   // Upper limit of integration
    int numTrapezoids = 1000; // Number of trapezoids

    double result = trapezoidalRule(lowerLimit, upperLimit, numTrapezoids);

    printf("Approximate integral of f(x) from %lf to %lf = %lf\n", lowerLimit, upperLimit, result);

    return 0;
}
