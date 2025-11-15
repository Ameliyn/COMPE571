#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>

#define mW_to_Jps = 0.001 //Milliwatts to Joules Per Second

struct process_description{
	//<name of task> <deadline/period> <WCET @ 1188 Mhz> <WCET @918 Mhz> <WCET @ 648 Mhz> <WCET @ 384 Mhz>.
	char name[8];
	int period, wcet[4];
};

/**
 * Generates a Rate Monotonic Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * task_mask (int[]): Execution Time Selection
 * pow (int[]): CPU Power at Frequency (mW)
 * print_results (int): Flag to print results
 * 
 * Returns:
 * float: Joules cost of schedule
 */
float generate_rm_schedule(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int task_mask[], 
	int pow[],
	int print_results) 
{
	printf("Scheduling %d tasks using RM\n", num_tasks);
	return 0;
}

/**
 * Generates an Energy Efficient Rate Monotonic Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * pow (int[]): CPU Power at Frequency (mW)
 * print_results (int): Flag to print results
 * 
 * Returns:
 * float: Joules cost of schedule
 */
int generate_ee_rm_schedule(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int pow[],
	int print_results) 
{
	printf("Scheduling %d tasks using RM EE\n", num_tasks);
	return 0;
}

/**
 * Generates an Earliest Deadline First Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * task_mask (int[]): Execution Time Selection
 * pow (int[]): CPU Power at Frequency (mW)
 * print_results (int): Flag to print results
 * 
 * Returns:
 * float: Joules cost of schedule
 */
float generate_edf_schedule(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int task_mask[], 
	int pow[],
	int print_results) 
{

	printf("Scheduling %d tasks using EDF\n", num_tasks);
	return 0;
}

/**
 * Generates an Energy Efficent Earliest Deadline First Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * pow (int[]): CPU Power at Frequency (mW)
 * print_results (int): Flag to print results
 * 
 * Returns:
 * float: Joules cost of schedule
 */
int generate_ee_edf_schedule(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int pow[],
	int print_results){
	printf("Scheduling %d tasks using EE\n", num_tasks);
	return 0;
}

int main(int argc, char const *argv[])
{
	if(argc < 3){
		printf("USAGE: ./a.out INPUT_FILENAME SCHEDULING_ALGORITHM\n");
		printf("Scheduling Algorithms: RM, EDF, RM EE, EDF EE\n");
		exit(1);
	}

	const char * input_filename = argv[1];

	int rm = 1;
	if(strcmp("EDF", argv[2]) == 0){
		rm = 0;
	}
	int ee = 0;
	if(argc > 3 && strcmp("EE", argv[3]) == 0){
		ee = 1;
	}

	if(rm){
		printf("Analyzing %s and scheduling with RM (Energy Efficient: %d)\n", 
			input_filename, ee);
	}
	else{
		printf("Analyzing %s and scheduling with EDF (Energy Efficient: %d)\n", 
			input_filename, ee);
	}

	FILE *fpointer = fopen(input_filename, "r");
	if (fpointer == NULL) {
        printf("Error: Could not open file.\n");
        exit(1);
    }

	int num_tasks, sys_time, pow[5];

	fscanf(fpointer, "%d %d %d %d %d %d %d",
		&num_tasks, &sys_time, &pow[0], &pow[1], &pow[2], &pow[3], &pow[4]);
	
	printf("Num_tasks: %d over %d seconds.\nCPU Power Costs:\n",num_tasks, sys_time);
	printf("  1188: %4dmW\n  918 : %4dmW\n  648 : %4dmW\n  384 : %4dmW\n  idle: %4dmW\n",
		pow[0], pow[1], pow[2], pow[3], pow[4]);
	
	printf("Tasks: %8s %6s %4d %4d %4d %4d\n","Name", "Period",1188,918,648,384);
		struct process_description tasks[num_tasks]; 
	int result;

	int task_mask[num_tasks];
	for(int i = 0; i < num_tasks; i++){
		result = fscanf(fpointer, "%8s %d %d %d %d %d",
		tasks[i].name, 
		&tasks[i].period, 
		&tasks[i].wcet[0], &tasks[i].wcet[1], &tasks[i].wcet[2], &tasks[i].wcet[3]);
		
		if (result != 6){
			printf("Error Reading File...\n(NOTE: TASK_NAME_MAX_LENGTH=8)\n");
			exit(1);
		}
		task_mask[i] = 0;
		printf("      %8s %6d  %4d %4d %4d %4d\n",
		tasks[i].name, 
		tasks[i].period, 
		tasks[i].wcet[0], tasks[i].wcet[1], tasks[i].wcet[2], tasks[i].wcet[3]);
	}
	fclose(fpointer);

	if(rm){
		if(ee){
			generate_ee_rm_schedule(num_tasks, sys_time, tasks, pow, 1);
		}
		else{
			generate_rm_schedule(num_tasks, sys_time, tasks, task_mask, pow, 1);
		}
	}
	else{
		if(ee){
			generate_ee_edf_schedule(num_tasks, sys_time, tasks, pow, 1);
		}
		else{
			generate_edf_schedule(num_tasks, sys_time, tasks, task_mask, pow, 1);
		}
	}
}