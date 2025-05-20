#include <stdio.h>
#include <omp.h>

int main() {
	omp_set_num_threads(2);

	int N;
	
	printf("Enter The Number Of Fibonacci Numbers To Calculate: \n");
	scanf("%d", &N);
	
	if (N < 0) {
		printf("Invalid Number Entered, Enter A Positive Number \n");
	}
	else {
		int fibo[N];
		
		#pragma omp parallel
		#pragma omp sections
		{
			#pragma omp section
			{
				if (N == 1) {
					fibo[0] = 0;
				}
			}
		
			#pragma omp section
			{
				if (N == 2) {
					fibo[0] = 0;
					fibo[1] = 1;
				}
			}
		
			#pragma omp section
			{
				if (N > 2) {
					fibo[0] = 0;
					fibo[1] = 1;
					for (int i = 2; i < N; i++) {
						fibo[i] = fibo[i - 2] + fibo[i - 1];
					}
				}
			}
		
			#pragma omp section
			{
				printf("First %d Fibonacci Numbers : [", N);
			}
		}

		// You cannot use `section` inside a loop. So print serially here.
		for (int j = 0; j < N; j++) {
			if (j != N - 1) {
				printf("%d, ", fibo[j]);
			}
			else {
				printf("%d] \n", fibo[j]);
			}
		}
	}
	
	return 0;
}

