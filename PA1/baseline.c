#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    struct timespec start, end;

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    int N = atoi(argv[1]);
    unsigned long sum = 0;
    for(int i = 1; i <= N; i++){sum += i;} 
    
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    printf("The sum of integers from 1 to %d is: %lu\n", N, sum);
    printf("This operation took %0.5f seconds.\n", time_taken);

}