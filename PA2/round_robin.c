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

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

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
    struct timespec start, end, proc_end[4], temp1, temp2;
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

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0){
		    clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			time_taken[0] += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;
		}
		if (running2 > 0){
		    clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			time_taken[1] += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;
		}
		if (running3 > 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			time_taken[2] += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;

		}
		if (running4 > 0){
		    clock_gettime(CLOCK_MONOTONIC_RAW, &temp1);
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC_RAW, &temp2);
			time_taken[3] += (temp2.tv_sec - temp1.tv_sec) + (double)(temp2.tv_nsec - temp1.tv_nsec) / 1000000000;
		}

		waitpid(pid1, &running1, WNOHANG);
		if (running1 == 0 && proc_end[0].tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &proc_end[0]);
		}
		waitpid(pid2, &running2, WNOHANG);
		if (running2 == 0 && proc_end[1].tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &proc_end[1]);
		}
		waitpid(pid3, &running3, WNOHANG);
		if (running3 == 0 && proc_end[2].tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &proc_end[2]);
		}
		waitpid(pid4, &running4, WNOHANG);
		if (running4 == 0 && proc_end[3].tv_nsec == 0){
			clock_gettime(CLOCK_MONOTONIC_RAW, &proc_end[3]);
		}
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