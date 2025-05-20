#include <stdio.h>
#include <mpi.h>

int main(int arg, char **argv) {
	int num_procs;
	int rank;
	
	int data[4] = {2,4,6,8};
	int duplicate[4];
	
	MPI_Init(&arg, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	printf("Rank %d : Number Of Processors Running : %d \n", rank, num_procs);
	
	if (rank == 0) {
		printf("Rank %d sending to Rank %d \n", rank, rank+1);
		MPI_Send(data, 4, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("Rank %d : Data Sent \n", rank);
	}
	
	else if (rank == 1) {
		printf("Rank %d receiving from Rank %d \n", rank, rank-1);
		MPI_Recv(duplicate, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Rank %d : Data Received \n", rank);
		
		for (int i=0; i<4; i++) {
			printf("%d \n", duplicate[i]);
		}
	}
	
	else {
		printf("Rank %d: No role in this communication.\n", rank);
	}
	
	MPI_Finalize();
	
	return 0;
}
