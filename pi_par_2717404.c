#include <stdio.h>
#include <time.h>
#include <omp.h>

static long num_steps = 1000000;
double step;

double sequential_pi(double *cpu_time_used) {
    clock_t start, end;
    double sum = 0.0, pi;
    step = 1.0 / (double)num_steps;
    
    start = clock();
    for (int i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    end = clock();
    
    *cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return pi;
}

double parallel_false_sharing_pi(int *nthreads, double *cpu_time_used) {
    clock_t start, end;
    step = 1.0 / (double)num_steps;
    int num_threads = omp_get_max_threads();
    double sum[num_threads];
    
    start = clock();
    
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int tthreads = omp_get_num_threads();
        if (id == 0)
            *nthreads = tthreads;
        sum[id] = 0.0;
        
        for (int i = id; i < num_steps; i += tthreads) {
            double x = (i + 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }
    
    double pi = 0.0;
    for (int i = 0; i < *nthreads; i++) {
        pi += step * sum[i];
    }
    
    end = clock();
    *cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return pi;
}

double parallel_race_condition_pi(int *nthreads, double *cpu_time_used) {
    clock_t start, end;
    double sum = 0.0;
    step = 1.0 / (double)num_steps;
    int num_threads = omp_get_max_threads();
    
    start = clock();
    
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int tthreads = omp_get_num_threads();
        if (id == 0)
            *nthreads = tthreads;

        for (int i = id; i < num_steps; i += tthreads) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    }
    
    double pi = step * sum;
    end = clock();
    *cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return pi;
}

double parallel_atomic_pi(int *nthreads, double *cpu_time_used) {
    clock_t start, end;
    double sum = 0.0;
    step = 1.0 / (double)num_steps;
    int num_threads = omp_get_max_threads();
    
    start = clock();
    
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int tthreads = omp_get_num_threads();
        if (id == 0)
            *nthreads = tthreads;
        double partial_sum = 0.0;
        
        for (int i = id; i < num_steps; i += tthreads) {
            double x = (i + 0.5) * step;
            partial_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        sum += partial_sum;
    }
    
    double pi = step * sum;
    end = clock();
    *cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return pi;
}

int main() {
    double seq_time, cpu_time_used;
    int nthreads = 0;
    double pi, speedup;
    
    pi = sequential_pi(&cpu_time_used);
    seq_time = cpu_time_used;
    printf("Sequential: pi with 1000000 steps is %f in %f seconds\n", pi, cpu_time_used);
    printf("\n");
    
    
    pi = parallel_false_sharing_pi(&nthreads, &cpu_time_used);
    speedup = seq_time / cpu_time_used;
    printf("Parallel with false sharing: pi with 1000000 steps is %f in %f seconds using %d threads\n", pi, cpu_time_used, nthreads);
    printf("Speedup: %f\n", speedup);
    printf("\n");
    

    pi = parallel_race_condition_pi(&nthreads, &cpu_time_used);
    speedup = seq_time / cpu_time_used;
    printf("Parallel with race condition: pi with 1000000 steps is %f in %f seconds using %d threads\n", pi, cpu_time_used, nthreads);
    printf("Speedup: %f\n", speedup);
    printf("\n");
    

    pi = parallel_atomic_pi(&nthreads, &cpu_time_used);
    speedup = seq_time / cpu_time_used;
    printf("Parallel with no race and atomic update: pi with 1000000 steps is %f in %f seconds using %d threads\n", pi, cpu_time_used, nthreads);
    printf("Speedup: %f\n", speedup);
    
    return 0;
}

