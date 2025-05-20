#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, num_procs;
    int N;  // Number of hops
    char token[7] = "Hello!";

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (rank == 0) {
        printf("Enter the number of times to pass the message: ");
        scanf("%d", &N);
    }

    // Broadcast N to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (int count = 0; count < N; count++) {
        int sender = count % num_procs;
        int receiver = (sender + 1) % num_procs;

        if (rank == sender) {
            MPI_Send(token, strlen(token) + 1, MPI_CHAR, receiver, 0, MPI_COMM_WORLD);
            printf("Process %d sent '%s' to Process %d\n", sender, token, receiver);
        }

        if (rank == receiver) {
            MPI_Recv(token, 7, MPI_CHAR, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d received '%s' from Process %d\n", receiver, token, sender);
        }

        MPI_Barrier(MPI_COMM_WORLD);  // Sync all before next iteration
    }

    MPI_Finalize();
    return 0;
}

