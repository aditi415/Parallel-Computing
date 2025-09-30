#include<iostream>
#include<mpi.h>
#include<vector>
using namespace std;

int main(int argc, char *argv[])
{
	MPI_Init(&argc,&argv);
	MPI_Status status;
	int rank,size; 

	MPI_Comm_rank(MPI_COMM_WORLD,& rank);
	MPI_Comm_size(MPI_COMM_WORLD,& size);
	
         int n=100;
	 int part=n/size;
	 if(rank==0){
	   vector<int>A;
   		for(int i=1;i<=n;i++){
			A.push_back(i);
		   }
	 MPI_Send(A.data()+part,part,MPI_INT,1,1,MPI_COMM_WORLD);
 	 int sum=0;
	 for(int i=0;i<part;i++){
		 sum+=A[i];
	    }
	    cout<< rank<< sum <<endl;
	 }

	if(rank!=0){
        	vector<int>B(part);
             MPI_Recv(B.data(),part,MPI_INT,0,1,MPI_COMM_WORLD,&status);
         	for(int i=0;i<part;i++){
	        	cout<<B[i]<<endl;
	      }

         int lsum=0;
	 for(int i=0;i<part;i++){
		 lsum+=B[i];
	    }
	    cout<< rank<<"" << lsum <<endl;
	 }

	MPI_Finalize();
	return 0;

}
