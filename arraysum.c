#include <stdio.h>
#include <mpi.h>

int main(int arg, char **argv) {
	
	int arr[10] = {2,4,6,8,10,12,14,16,18,20};
	
	int sub_arr[2];
	
	int partsum = 0;
	
	int totalsum = 0;
	
	int num_procs;
	int rank;
	
	MPI_Init(&arg, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	MPI_Scatter(arr, 2, MPI_INT, sub_arr, 2, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (int i=0; i<2; i++) {
		partsum += sub_arr[i];
	}
	
	//printf("Rank %d : PartSum is %d \n", rank, partsum);
	
	MPI_Reduce(&partsum, &totalsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (rank == 0) {
		printf("Total Sum Is %d \n", totalsum);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	MPI_Finalize();
	
	return 0;
}
