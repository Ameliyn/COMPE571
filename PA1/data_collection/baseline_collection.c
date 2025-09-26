#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int main(int argc, char *argv[]){
    struct timespec start, end;
    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    char * endptr = "";
    long long N = strtoll(argv[1], &endptr, 10);
    unsigned long sum = 0;
    for(long long i = 1; i < N; i++){
        sum += i;
    } 
    
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    printf("%lu, %lf\n", sum, time_taken);
}