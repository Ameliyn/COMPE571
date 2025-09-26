#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int calculate_sum(unsigned long long lower, unsigned long long upper){
    unsigned long long sum = 0;

    for(unsigned long long i = lower; i <= upper; i++){
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
    unsigned long long N = strtoll(argv[1], &endptr, 10);
    int NUM_TASKS = atoi(argv[2]);
    unsigned long long sum = 0;

    int pipes[NUM_TASKS][2];    
    pid_t tasks[NUM_TASKS];


    // Divide up the work
    if (NUM_TASKS > N){
        printf("WARNING: NUMTASKS (%d) > N (%llu). Setting NUMTASKS to %llu\n", NUM_TASKS, N, N);
        NUM_TASKS = N;
    }
    unsigned long long separation = N / NUM_TASKS;
    unsigned long long thread_data[2];
    int i;
    for(i=0; i < NUM_TASKS; i++){
        int status = pipe(pipes[i]);
        thread_data[0] = i * separation + 1;
        thread_data[1] = (i+1) * separation;

        if (i+1 == NUM_TASKS){
            thread_data[1] = N;
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
    unsigned long long temp_sum;
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
    printf("%llu, %lf\n", sum, time_taken);
}