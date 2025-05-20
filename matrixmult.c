#include <stdio.h>
#include <omp.h>

int main() {
	int N;
	int M;
	
	printf("Enter N & M: \n");
	
	scanf("%d", &N);
	scanf("%d", &M);

	int A[N][M];
	int B[M][N];
	int C[N][N];
	int num;
	
	printf("Enter matrix A elements: \n");
	
	for (int a_row=0; a_row<N; a_row++) {
		for (int a_col=0; a_col<M; a_col++) {
			scanf("%d", &num);    // Takes in a number and saves it in the variable num
			A[a_row][a_col] = num;
		}
	}
	
	printf("Enter matrix B elements: \n");
	
	for (int b_row=0; b_row<M; b_row++) {
		for (int b_col=0; b_col<N; b_col++) {
			scanf("%d", &num);
			B[b_row][b_col] = num;
		}
	}
	
	omp_set_num_threads(2);
	
	#pragma omp parallel
	
	{
	
		#pragma omp for collapse(2)
	
		for (int i=0; i<N; i++) {
			for (int j=0; j<N; j++) {
				C[i][j] = 0;
				for (int k=0; k<M; k++) {
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	
	}
	
	for (int c_row=0; c_row<N; c_row++) {
		for (int c_col=0; c_col<N; c_col++) {
			printf("%d ", C[c_row][c_col]);
		}
		printf("\n");
	}
	
	return 0;
	
}
