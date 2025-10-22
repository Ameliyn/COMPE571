#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>

/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task and 
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

/************************************************************************************************ 
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/

struct process_description{
	int workload_time;
	pid_t pid;
	int id;
	struct timespec end_time;
	double processing_time;
};

int main(int argc, char const *argv[])
{
	if(argc < 2){
		printf("USAGE: ./a.out QUANTUM\n");
		exit(1);
	}
	int QUANTUM = atoi(argv[1]);
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;

	// Create all four processes
	pid1 = fork();
	if (pid1 == 0){
		myfunction(WORKLOAD1);
		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();
	if (pid2 == 0){
		myfunction(WORKLOAD2);
		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();
	if (pid3 == 0){
		myfunction(WORKLOAD3);
		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();
	if (pid4 == 0){
		myfunction(WORKLOAD4);
		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/
	// Store process information in a struct
	struct process_description processes[4], temp_process;
	for(int i = 0; i < 4; i++){
		processes[i].end_time.tv_nsec = 0; 
		processes[i].processing_time = 0;
		processes[0].id = i+1;
	}
	processes[0].workload_time = WORKLOAD1;
	processes[0].pid = pid1;

	processes[1].workload_time = WORKLOAD2;
	processes[1].pid = pid2;

	processes[2].workload_time = WORKLOAD3;
	processes[2].pid = pid3;

	processes[3].workload_time = WORKLOAD4;
	processes[3].pid = pid4;

	// Start timing
    struct timespec start, end, proc_end[4], temp1, temp2, proc_start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    clock_gettime(CLOCK_MONOTONIC_RAW, &proc_start);


	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0){
		    clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid1, SIGCONT);
			usleep(QUANTUM);
			kill(pid1, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			processes[0].processing_time += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;
		}
		if (running2 > 0){
		    clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid2, SIGCONT);
			usleep(QUANTUM);
			kill(pid2, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			processes[1].processing_time += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;
		}
		if (running3 > 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid3, SIGCONT);
			usleep(QUANTUM);
			kill(pid3, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			processes[2].processing_time += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;

		}
		if (running4 > 0){
		    clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid4, SIGCONT);
			usleep(QUANTUM);
			kill(pid4, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			processes[3].processing_time += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;
		}

		waitpid(pid1, &running1, WNOHANG);
		if (running1 == 0 && processes[0].end_time.tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &processes[0].end_time);
		}
		waitpid(pid2, &running2, WNOHANG);
		if (running2 == 0 && processes[1].end_time.tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &processes[1].end_time);
		}
		waitpid(pid3, &running3, WNOHANG);
		if (running3 == 0 && processes[2].end_time.tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &processes[2].end_time);
		}
		waitpid(pid4, &running4, WNOHANG);
		if (running4 == 0 && processes[3].end_time.tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &processes[3].end_time);
		}
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
	
	// Record End time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	
	double turnaround_time[4];
	for(int i = 0; i < 4; i++){
		turnaround_time[i] = (processes[i].end_time.tv_sec - start.tv_sec) + (double)(processes[i].end_time.tv_nsec - start.tv_nsec) / 1000000000;
	}

	double total_time = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
	double scheduling_time = (proc_start.tv_sec - start.tv_sec) + (double)(proc_start.tv_nsec - start.tv_nsec) / 1000000000;
	double context_switch_time = (total_time - scheduling_time
		- processes[0].processing_time - processes[1].processing_time 
		- processes[2].processing_time - processes[3].processing_time);
	
	// Print Results
    // printf("Turnaround Times: {%0.5f, %0.5f, %0.5f, %0.5f}.\n", turnaround_time[0], 
		// turnaround_time[1], turnaround_time[2], turnaround_time[3]);
    // printf("Processing Times: {%0.5f, %0.5f, %0.5f, %0.5f}.\n", processes[0].processing_time, 
		// processes[1].processing_time, processes[2].processing_time, processes[3].processing_time);
	// printf("Context Switch Time: %0.8f\n", context_switch_time);
	// printf("This operation took %0.5f seconds.\n", total_time);
	
	double avg_resp_time = (turnaround_time[0] + turnaround_time[1] + turnaround_time[2] + turnaround_time[3]) / 4; 
	// printf("Average Response Time: %0.8f\n", avg_resp_time);
	printf("%d, %0.8f, %0.8f\n", QUANTUM, avg_resp_time, context_switch_time);

	return 0;
}