#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


long calculate_sum(long lower, long upper){
    long sum = 0;

    for(long i = lower; i <= upper; i++){
        sum += i;
    }
    return sum;
}


int main(int argc, char *argv[]){
    struct timespec start, end;
    if (argc < 5){
        printf("USAGE: ./multitask_popen.o N_LOWER N_UPPER NUM_TASKS PARENT_FLAG\n");
        exit(1);
    }
    // START OF OPERATION
    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    char * endptr = "";
    long LOWER = strtol(argv[1], &endptr, 10);
    long UPPER = strtol(argv[2], &endptr, 10);
    int NUM_TASKS = atoi(argv[3]);
    int parent = atoi(argv[4]);
    long sum = 0;

    FILE * tasks[NUM_TASKS];

    // Just do the work and report it
    if(!parent && NUM_TASKS == 1){
        printf("%ld", calculate_sum(LOWER, UPPER));
        exit(0);
    }

    // Divide up the work
    if (NUM_TASKS > (UPPER - LOWER)){
        // printf("WARNING: NUMTASKS (%d) > N (%d). Setting NUMTASKS to %d\n", NUM_TASKS, N, N);
        NUM_TASKS = (UPPER - LOWER);
    }
    long separation = ((UPPER - LOWER) - 1) / NUM_TASKS;
    long thread_data[2];
    int i;
    for(i=0; i < NUM_TASKS; i++){
        // int status = pipe(pipes[i]);
        thread_data[0] = (long)i * separation + 1;
        thread_data[1] = ((long)i+1) * separation;

        if (i+1 == NUM_TASKS){
            thread_data[1] = UPPER - 1;
        }
        char command[100];
        sprintf(command, "./multitask_popen.o %ld %ld 1 0", thread_data[0], thread_data[1]);
        // printf("Creating child with %s\n", command);
        tasks[i] = popen(command, "r");
    }

    // Sum the data
    int status;
    long temp_sum;
    for(int i=0; i < NUM_TASKS; i++){
        // waitpid(tasks[i], &status, 0);
        status = fscanf(tasks[i], "%ld", &temp_sum);
        sum += temp_sum;
        pclose(tasks[i]);
    }

    // END OF OPERATION
    // Record the end time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    // Print results and time
    double time_taken = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    //Print the Results
    // printf("The sum of integers from 1 to %ld is: %ld\n", UPPER, sum);
    // printf("This operation took %0.5f seconds.\n", time_taken);
    printf("%ld, %lf\n", sum, time_taken);

}