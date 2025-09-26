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

    // START OF OPERATION
    // Record the start time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Do Work
    char * endptr = "";
    long N = strtol(argv[1], &endptr, 10);
    int NUM_TASKS = atoi(argv[2]);
    long sum = 0;

    int pipes[NUM_TASKS][2];    
    pid_t tasks[NUM_TASKS];


    // Divide up the work
    if (NUM_TASKS > N){
        // printf("WARNING: NUMTASKS (%d) > N (%d). Setting NUMTASKS to %d\n", NUM_TASKS, N, N);
        NUM_TASKS = N;
    }
    long separation = (N-1) / NUM_TASKS;
    long thread_data[2];
    int i;
    for(i=0; i < NUM_TASKS; i++){
        int status = pipe(pipes[i]);
        thread_data[0] = i * separation + 1;
        thread_data[1] = (i+1) * separation;

        if (i+1 == NUM_TASKS){
            thread_data[1] = N-1;
        }
        tasks[i] = fork();
        if(tasks[i] == 0){
            break;
        }
    }

    if (i != NUM_TASKS){
        sum = calculate_sum(thread_data[0], thread_data[1]);
        // printf("Child did processing sum = %ld\n", sum);

        //SEND BACK THE SUM
        close(pipes[i][0]);
        write(pipes[i][1], &sum, sizeof(sum));
        close(pipes[i][1]);
        exit(0);
    }

    // Sum the data
    int status;
    long temp_sum;
    for(int i=0; i < NUM_TASKS; i++){
        waitpid(tasks[i], &status, 0);
        
        close(pipes[i][1]);
        status = read(pipes[i][0], &temp_sum, sizeof(temp_sum));
        sum += temp_sum;
        close(pipes[i][0]);
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