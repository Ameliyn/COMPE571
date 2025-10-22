#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>
#include <linux/time.h>

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
};
int main(int argc, char const *argv[])
{
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


	// Start timing
    struct timespec start, end, proc_end[4], temp1;
	proc_end[0].tv_nsec = 0;
	proc_end[1].tv_nsec = 0;
	proc_end[2].tv_nsec = 0;
	proc_end[3].tv_nsec = 0;
	double time_taken[4] = {0,0,0,0};
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/

	// Find Scheduling Order

	struct process_description processes[4];
	processes[0].workload_time = WORKLOAD1;
	processes[0].pid = pid1;

	processes[1].workload_time = WORKLOAD2;
	processes[1].pid = pid2;

	processes[2].workload_time = WORKLOAD3;
	processes[2].pid = pid3;

	processes[3].workload_time = WORKLOAD4;
	processes[3].pid = pid4;

	// Sort by shortest job first
	for(int i = 0; i < 4; i++){
		if(processes[i].workload_time > processes[i+1].workload_time){
			struct process_description temp_process;
			temp_process.workload_time = processes[i+1].workload_time;
			temp_process.pid = processes[i+1].pid;
			processes[i+1].workload_time = processes[i].workload_time;
			processes[i+1].pid = processes[i].pid;
			processes[i].workload_time = temp_process.workload_time;
			processes[i].pid = temp_process.pid;
		}
		else {
			break;
		}
	}

	int status;
	for(int i = 0; i < 4; i++){
		clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
		kill(pid1, SIGCONT);
		waitpid(processes[i].pid, &status, 0);
		clock_gettime(CLOCK_MONOTONIC_RAW, &proc_end[i]);
		time_taken[i] += (proc_end[i].tv_sec - temp1.tv_sec) + (double)(proc_end[i].tv_nsec - temp1.tv_nsec) / 1000000000;
	}
	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
	
	// Record End time
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	
	double proc_time_taken[4] = {0,0,0,0};
	for(int i = 0; i < 4; i++){
		proc_time_taken[i] = (proc_end[i].tv_sec - start.tv_sec) + (double)(proc_end[i].tv_nsec - start.tv_nsec) / 1000000000;
	}
	double total_time = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
    double overhead_time = total_time - time_taken[0] - time_taken[1] - time_taken[2] - time_taken[3];
	
	// Print Results
    printf("Turnaround Times: {%0.5f, %0.5f, %0.5f, %0.5f}.\n", proc_time_taken[0], proc_time_taken[1], proc_time_taken[2], proc_time_taken[3]);
    printf("Processing Times: {%0.5f, %0.5f, %0.5f, %0.5f}.\n", time_taken[0], time_taken[1], time_taken[2], time_taken[3]);
	printf("Overhead: %0.5f\n", overhead_time);
	printf("This operation took %0.5f seconds.\n", total_time);

	return 0;
}