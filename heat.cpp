#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
const int ROWS = 400;
const int COLS = 400;
const int MAX_CYCLES = 1000;
const double INITIAL_TEMP = 40.0;
const double BOUNDARY_TEMP = 100.0;

int main()
{ // main opening
    cout << "**************************************************" << endl;
    cout << "Welcome to our Parallel Computing(441) Project!" << endl;
    cout << "**************************************************" << endl;

    vector<vector<double>> temperature(ROWS, vector<double>(COLS, INITIAL_TEMP));

    // Setting boundary conditions
    for (int i = 0; i < ROWS; ++i)
    {                                             // for opening
        temperature[i][0] = BOUNDARY_TEMP;        // Left side
        temperature[i][COLS - 1] = BOUNDARY_TEMP; // Right side
    }                                             // for closing

    for (int j = 0; j < COLS; ++j)
    {                                             // for opening
        temperature[0][j] = BOUNDARY_TEMP;        // Top side
        temperature[ROWS - 1][j] = BOUNDARY_TEMP; // Bottom side
    }                                             // for closing

    // Simulation loop
    auto start_time = chrono::high_resolution_clock::now();

    for (int cycle = 0; cycle < MAX_CYCLES; ++cycle)
    { // for opening
        vector<vector<double>> newTemperature(ROWS, vector<double>(COLS, 0.0));

        for (int i = 1; i < ROWS - 1; ++i)
        { // for opening
            for (int j = 1; j < COLS - 1; ++j)
            { // for opening
                newTemperature[i][j] = 0.25 * (temperature[i - 1][j] + temperature[i + 1][j] +
                                               temperature[i][j - 1] + temperature[i][j + 1]);
            } // inner for closing
        }     // outer for closing

        temperature = newTemperature; // Update the temperature grid
    }                                 // for closing (cycle)

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    // Find the max and min temperature in the grid
    double maxTemp = INITIAL_TEMP;
    double minTemp = INITIAL_TEMP;
    for (int i = 0; i < ROWS; ++i)
    { // for opening
        for (int j = 0; j < COLS; ++j)
        { // for opening
            maxTemp = max(maxTemp, temperature[i][j]);
            minTemp = min(minTemp, temperature[i][j]);
        } // inner for closing
    }     // outer for closing

    // Output the results
    cout << "Maximum temperature: " << maxTemp << " degrees" << endl;
    cout << "Minimum temperature: " << minTemp << " degrees" << endl;
    cout << "Time taken (ms): " << duration << " ms" << endl;
    cout << "---------------uob---------------------" << endl;

    return 0; // returning from main
} // main closing
