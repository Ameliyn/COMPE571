#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int NUM_THREADS;

/**
* Calculate the sum of the integers [lower, upper]
*/
void calculate_sum(void * args){
    int *data = (int * )args;
    int sum = 0;

    for(int i = data[0]; i <= data[1]; i++){
        sum += i;
    }
    // printf("Thread calculation complete: [%d, %d] = %d\n", data[0], data[1], sum);
    data[2] = sum;
}

int main(int argc, char *argv[]){
    struct timespec start, end;

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    int N = atoi(argv[1]);
    NUM_THREADS = atoi(argv[2]);
    
    pthread_t threads[NUM_THREADS];

    int sum = 0;

    // Divide up the work
    int separation = N / NUM_THREADS;

    if (NUM_THREADS > N){
        separation = 1;
        NUM_THREADS = N;
    }
    int args[NUM_THREADS][3];

    // printf("Creating %d threads with %d separation.\n", NUM_THREADS, separation);
    
    for(int i=0; i < NUM_THREADS; i++){
        args[i][0] = i * separation + 1;
        args[i][1] = (i+1) * separation;

        if (i+1 == NUM_THREADS){
            args[i][1] = N;
        }
        printf("Creating thread for [%d,%d]\n", args[i][0], args[i][1]);
        pthread_create(&threads[i], NULL, calculate_sum, (void *)args[i]);
    }
    
    // Sum the data
    printf("Joining Threads.\n");
    for(int i=0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
        sum += args[i][2];
    }

    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    printf("The sum of integers from 1 to %d is: %d\n", N, sum);
    printf("This operation took %0.5f seconds.\n", time_taken);

}