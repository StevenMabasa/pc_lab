#include <stdio.h>
#include <omp.h>

int main() {
	omp_set_num_threads(4);
	
	static long num_steps = 1000000;
	double step;
	int i;
	double x, pi, sum=0.0;
	
	step = 1.0/(double)num_steps;
	
	#pragma omp parallel
	
	{
	
		#pragma omp for reduction (+:sum)
	
		for (i=0; i<num_steps; i++) {
			x = ((double)(i+0.5))*step;
			sum += 4.0/(1.0+x*x);
		}
	
	}
	
	pi = step*sum;
	
	printf("%.24f\n",pi);
	
	
	printf("%d\n",omp_get_num_procs());
	
	return 0;
}
