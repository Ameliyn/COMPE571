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
    long long sum = 0;
    int bignum = 0;
    for(long long i = 1; i < N; i++){
        if(sum + i < 0){
            printf("Overflow!\n");
            bignum += 1;
            sum -= 1000000000000000000;    
        }
        sum += i;
    } 
    
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    printf("%d%llu, %lf\n", bignum, sum, time_taken);
}