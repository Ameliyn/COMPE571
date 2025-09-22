#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    struct timespec start, end;

    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    int N = atoi(argv[1]);
    int sum = 0;
    for(int i = 1; i <= N; i++){sum += i;} 
    
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    printf("%d %lf\n", sum, time_taken);

}