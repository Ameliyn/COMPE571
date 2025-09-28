#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void *calculate_sum(void * thread_data){
    long *data = (long * )thread_data;
    long sum = 0;

    for(long i = data[0]; i <= data[1]; i++){
        sum += i;
    }
    data[2] = sum;
    // pthread_exit();
}


int main(int argc, char *argv[]){
    struct timespec start, end;
    if (argc < 2){
        printf("USAGE: ./multithread.o N NUMTHREADS\n");
        exit(1);
    }
    // START OF OPERATION
    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    char * endptr = "";
    long N = strtol(argv[1], &endptr, 10);
    N = N-1;

    int NUM_THREADS = atoi(argv[2]);
    
    pthread_t threads[NUM_THREADS];

    long sum = 0;

    // Divide up the work
    if (NUM_THREADS > N){
        // printf("WARNING: NUMTHREADS (%d) > N (%d). Setting NUMTHREADS to %d\n", NUM_THREADS, N, N);
        NUM_THREADS = N;
    }
    long separation = N / NUM_THREADS;
    long thread_data[NUM_THREADS][3];

    for(int i=0; i < NUM_THREADS; i++){
        thread_data[i][0] = i * separation + 1;
        thread_data[i][1] = (i+1) * separation;

        if (i+1 == NUM_THREADS){
            thread_data[i][1] = N;
        }
        // printf("Creating thread for [%d,%d]\n", thread_data[i][0], thread_data[i][1]);
        pthread_create(&threads[i], NULL, calculate_sum, (void *)thread_data[i]);
    }
    
    // Sum the data
    for(int i=0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
        sum += thread_data[i][2];
    }

    // END OF OPERATION
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // Print results and time
    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    printf("The sum of integers from 1 to %ld is: %ld\n", N, sum);
    printf("This operation took %0.5f seconds.\n", time_taken);

}