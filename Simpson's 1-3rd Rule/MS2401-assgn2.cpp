#include <iostream>
#include <cmath>
#include <mpi.h>
using namespace std;

// Function f(x) = x^2 * e^(-x)
double f(double x) {
    return x * x * exp(-x);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    MPI_Status status;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Integration limits
    double a = 0.0, b = 40.0;

    // Values of n to test
    int intervals[] = {12, 20, 40, 80};

    for (int test = 0; test < 4; test++) {
        int n = intervals[test];  // choose n
        double h = (b - a) / n;

        // Divide work among processes
        int part = n / size;
        int start = rank * part + 1;
        int end   = (rank + 1) * part;

        double lsum = 0.0;

        // Compute local sum for this rank
        for (int i = start; i <= end; i++) {
            double x = a + i * h;
            if (i % 3 == 0)
                lsum += 2 * f(x);
            else
                lsum += 3 * f(x);
        }

        if (rank == 0) {
            double globalsum = lsum;
            // Collect partial sums from other ranks
            for (int i = 1; i < size; i++) {
                double temp;
                MPI_Recv(&temp, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
                globalsum += temp;
            }
            // Add endpoints f(a) and f(b)
            globalsum += f(a) + f(b);
            // Final Simpson’s 3/8 formula
            globalsum = (3 * h / 8.0) * globalsum;

            cout << "Result for n = " << n << " : " << globalsum << endl;
        } else {
            // Send local sum to root
            MPI_Send(&lsum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
