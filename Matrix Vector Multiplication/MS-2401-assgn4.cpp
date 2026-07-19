#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) 
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 0, m = 0;
    vector<double> A, x;

    if (rank == 0) 
    {
        ifstream fin("input.txt");
        if (!fin.is_open())
        {
            cerr << "Error: Cannot open input.txt\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fin >> n >> m;
        A.resize(n * m);
        x.resize(m);

        for (int i = 0; i < n * m; i++) 
          fin >> A[i];
          for (int i = 0; i < m; i++) 
            fin >> x[i];
        fin.close();
  }
  
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n % size != 0) 
    {
      if(rank == 0)
      {    
        cout << "Warning: n not divisible by number of processes.\n";
      }
    }  

    int rows = n / size;

    vector<double> local_A(rows * m);
    vector<double> local_b(rows, 0.0);
  
   if (rank != 0) 
   {
      x.resize(m);
   }
    MPI_Scatter(A.data(), rows * m, MPI_DOUBLE,local_A.data(), rows * m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    MPI_Bcast(x.data(), m, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < m; j++)
        {
            sum += local_A[i * m + j] * x[j];
        }
        local_b[i] = sum;
    }
    
    vector<double> b;
    
    if (rank == 0)
    { 
      b.resize(n);
    }
    
    MPI_Gather(local_b.data(), rows, MPI_DOUBLE,b.data(), rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) 
    {
        ofstream fout("output.txt");
        fout << "Resultant Vector (b = A*x):\n";
        for (int i = 0; i < n; i++)
        {
          fout << b[i] << "\n";
        }
        fout.close();
        cout << "Output written to output.txt\n";
    }

    MPI_Finalize();
    return 0;
}

