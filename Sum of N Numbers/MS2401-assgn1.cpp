#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N;
    if (rank == 0) {
        cout << "Enter value of N: ";
        cin >> N;

        // Send N to all other processes
        for (int p = 1; p < size; p++) {
            MPI_Send(&N, 1, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    } else {
        // Receive N from root
        MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    // Each process calculates its share (cyclic distribution)
    int local_sum = 0;
    for (int i = rank + 1; i <= N; i += size) {
        local_sum += i;
    }

    if (rank == 0) {
        int total_sum = local_sum, temp_sum;

        // Collect local sums from workers
        for (int p = 1; p < size; p++) {
            MPI_Recv(&temp_sum, 1, MPI_INT, p, 1, MPI_COMM_WORLD, &status);
            total_sum += temp_sum;
        }

        cout << "Total sum of first " << N << " numbers = " << total_sum << endl;
    } else {
        // Send local sum to root
        MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
