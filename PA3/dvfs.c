#include <stdio.h> 
#include <stdlib.h>  
#include <string.h>

struct process_description{
	//<name of task> <deadline/period> <WCET @ 1188 Mhz> <WCET @918 Mhz> <WCET @ 648 Mhz> <WCET @ 384 Mhz>.
	char name[8];
	int period, wcet[4];
};

/**
 * Generates a Rate Monotonic Schedule for the provided tasks.
 * 
 * If the CPU utilization of the task set is greater than 1, then this function returns -1.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * task_mask (int[]): Execution Time Selection
 * cpu_power (int[]): CPU Power at Frequency (mW)
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
	int cpu_power[],
	int print_results) 
{
	const int frequencies[4] = {1188, 918, 648, 384};
	
	if(print_results) printf("Scheduling %d tasks using RM\n", num_tasks);
	// const double rm_constant = (double)num_tasks * (pow(2, 1.0/(double)num_tasks)-1);
	double task_utilization = 0.0;
	for(int i = 0; i < num_tasks; i++){
		task_utilization += ((double)tasks[i].wcet[task_mask[i]] / (double)tasks[i].period);
	}
	if (task_utilization > 1){
		if(print_results) printf("Tasks not schedulable using RM... %0.4lf > 1\n", task_utilization);
		return -1;
	}

	int remaining_computation[num_tasks];
	int active_task = -1, new_active_task = -1;
	double energy_consumed = 0;
	double total_energy_consumed = 0;
	int last_change_time, total_idle_time = 0;
	if(print_results) printf("%8s %8s %8s %8s %8s\n", "START", "NAME", "FREQ", "END", "POWER");
	for(int timestep = 0; timestep < sys_time; timestep++){

		// Check for missed deadlines
		for(int i = 0; i < num_tasks; i++){
			if(timestep != 0 && timestep % tasks[i].period == 0 && remaining_computation[i] > 0){
				if(print_results) printf("%d Missed their deadline. Scheduling Failed on timestep %d...\n", i, timestep);
				return -1;
			}
		}

		int scheduling_flag = 0;
		// Restart tasks as they come in.
		for(int i = 0; i < num_tasks; i++){
			if(timestep % tasks[i].period == 0){
				remaining_computation[i] = tasks[i].wcet[task_mask[i]];
				scheduling_flag = 1;
			}
		}

		if(active_task != -1 && remaining_computation[active_task] == 0){
			scheduling_flag = 1;
		}

		// Only check the scheduler if necessary
		if(scheduling_flag) {
			// Select current active task
			new_active_task = -1;
			for(int i = 0; i < num_tasks; i++){
				if(new_active_task == -1 && remaining_computation[i] > 0){
					new_active_task = i;
				}
				else if(remaining_computation[i] > 0 && remaining_computation[new_active_task] > remaining_computation[i]){
					new_active_task = i;
				}
			}
			if(new_active_task != active_task){
				// Compute Energy Consumed
				int time_taken = timestep - last_change_time;

				if(new_active_task == -1){

					if(timestep != 0){
						energy_consumed = (double)time_taken * ((double)cpu_power[4] * 0.001);
						total_idle_time += time_taken;
						total_energy_consumed += energy_consumed;
						if(print_results) printf(" %8d %8.4lfJ\n", timestep-1, energy_consumed);
					}
					if(timestep+1 != sys_time){
						if(print_results) printf("%8d %8s %8s", timestep, "IDLE", "IDLE");
					}
				}
				else{
					if (timestep != 0){
						energy_consumed = (double)time_taken * ((double)cpu_power[task_mask[new_active_task]] * 0.001);
						total_energy_consumed += energy_consumed;
						if(print_results) printf(" %8d %8.4lfJ\n", timestep-1, energy_consumed);

					}
					if(timestep+1 != sys_time){
						if(print_results) printf("%8d %8s %8d", timestep, tasks[new_active_task].name, frequencies[task_mask[new_active_task]]);
					}

				}
				last_change_time = timestep;
				active_task = new_active_task;
			}
		}

		if(active_task != -1) remaining_computation[active_task]--;	
	}
	int time_taken = sys_time - last_change_time;
	if(active_task != -1){
		energy_consumed = (double)time_taken * ((double)cpu_power[task_mask[new_active_task]] * 0.001);
		if(print_results) printf(" %8d %8.4lfJ\n", sys_time, energy_consumed);
	}
	else{
		total_idle_time += time_taken;
		energy_consumed = (double)time_taken * ((double)cpu_power[4] * 0.001);
		if(print_results) printf(" %8d %8.4lfJ\n", sys_time, energy_consumed);
	}
	total_energy_consumed += energy_consumed;
	if(print_results) {
		printf("Total Energy Consumed: %8.4lfJ\n", total_energy_consumed);
		printf("Total Idle time: %d seconds (%4.4f%%)\n", total_idle_time, (double)total_idle_time / (double) sys_time);
	}

	return total_energy_consumed;
}

/**
 * Generates an Earliest Deadline First Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * task_mask (int[]): Execution Time Selection
 * cpu_power (int[]): CPU Power at Frequency (mW)
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
	int cpu_power[],
	int print_results) 
{
	const int frequencies[4] = {1188, 918, 648, 384};
	
	if(print_results) {
		printf("Scheduling %d tasks using EDF\n", num_tasks);
	}
	double task_utilization = 0.0;
	for(int i = 0; i < num_tasks; i++){
		task_utilization += ((double)tasks[i].wcet[task_mask[i]] / (double)tasks[i].period);
	}
	if (task_utilization > 1){
		if(print_results) printf("Tasks not schedulable using EDF... Utiliztation %0.4lf > 1\n", task_utilization);
		return -1;
	}

	int remaining_computation[num_tasks], next_deadline[num_tasks];
	int active_task = -1, new_active_task = -1;
	double energy_consumed = 0;
	double total_energy_consumed = 0;
	int last_change_time, total_idle_time = 0;
	if(print_results) printf("%8s %8s %8s %8s %8s\n", "START", "NAME", "FREQ", "END", "POWER");
	for(int i = 0; i < num_tasks; i++){next_deadline[i] = tasks[i].period;}
	for(int timestep = 0; timestep < sys_time; timestep++){
		// Check for missed deadlines
		for(int i = 0; i < num_tasks; i++){
			if(timestep != 0 && timestep % tasks[i].period == 0 && remaining_computation[i] > 0){
				if(print_results) printf("\n%d Missed their deadline. Scheduling Failed on timestep %d...\n", i, timestep);
				return -1;
			}
		}

		int scheduling_flag = 0;
		// Restart tasks as they come in.
		for(int i = 0; i < num_tasks; i++){
			if(timestep % tasks[i].period == 0){
				remaining_computation[i] = tasks[i].wcet[task_mask[i]];
				next_deadline[i] = timestep + tasks[i].period;  
				scheduling_flag = 1;
			}
		}
		if(active_task != -1 && remaining_computation[active_task] == 0){
			scheduling_flag = 1;
		}

		// Only check the scheduler if necessary
		if(scheduling_flag){
			// Select current active task
			new_active_task = -1;
			for(int i = 0; i < num_tasks; i++){
				if(new_active_task == -1 && remaining_computation[i] > 0){
					new_active_task = i;
				}
				else if(remaining_computation[i] > 0 && next_deadline[i] < next_deadline[new_active_task]){
					new_active_task = i;
				}
			}
			if(new_active_task != active_task){
				// Compute Energy Consumed
				int time_taken = timestep - last_change_time;

				if(new_active_task == -1){

					if(timestep != 0){
						energy_consumed = (double)time_taken * ((double)cpu_power[4] * 0.001);
						total_energy_consumed += energy_consumed;
						total_idle_time += time_taken;
						if(print_results) printf(" %8d %8.4lfJ\n", timestep-1, energy_consumed);
					}
					if(timestep+1 != sys_time){
						if(print_results) printf("%8d %8s %8s", timestep, "IDLE", "IDLE");
					}
				}
				else{
					if (timestep != 0){
						energy_consumed = (double)time_taken * ((double)cpu_power[task_mask[new_active_task]] * 0.001);
						total_energy_consumed += energy_consumed;
						if(print_results) printf(" %8d %8.4lfJ\n", timestep-1, energy_consumed);

					}
					if(timestep+1 != sys_time){
						if(print_results) printf("%8d %8s %8d", timestep, tasks[new_active_task].name, frequencies[task_mask[new_active_task]]);
					}

				}
				last_change_time = timestep;
				active_task = new_active_task;
			}
		}
		if (active_task != -1) remaining_computation[active_task]--;	
	}
	int time_taken = sys_time - last_change_time;
	if(active_task != -1){
		energy_consumed = (double)time_taken * ((double)cpu_power[task_mask[new_active_task]] * 0.001);
		if(print_results) printf(" %8d %8.4lfJ\n", sys_time, energy_consumed);
	}
	else{
		total_idle_time += time_taken;
		energy_consumed = (double)time_taken * ((double)cpu_power[4] * 0.001);
		if(print_results) printf(" %8d %8.4lfJ\n", sys_time, energy_consumed);
	}
	total_energy_consumed += energy_consumed;
	if(print_results) {
		printf("Total Energy Consumed: %8.4lfJ\n", total_energy_consumed);
		printf("Total Idle time: %d seconds (%4.4f%%)\n", total_idle_time, (double)total_idle_time / (double) sys_time);
	}

	return total_energy_consumed;
}

/**
 * Recursively check every frequency for every task to get the best energy efficent schedule.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * cpu_power (int[]): CPU Power at Frequency (mW)
 * rm_flag (int): 1 for RM, 0 for EDF
 * task_pointer (int): Current task
 * task_mask (int[]): Array with frequency values for each task
 * best_task_mask (int[]): Current best task mask
 * best_power (float*): Float pointer that holds the lowest power task frequency set.
 * num_checked (int*): Debug value that increments for every RM schedule checked
 * 
 * NOTE: num_checked increments for every function call to "generate_rm_schedule". However, "generate_rm_schedule" checks the cpu
 * utilization and does not attempt to generate a complete RM schedule if that utilization is above 1. 
 * 
 * Returns:
 * float: Joules cost of schedule
 */
int generate_ee_schedule_helper(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int cpu_power[],
	int rm_flag,
	int task_pointer,
	int task_mask[],
	int best_task_mask[],
	float *best_power,
	int *num_checked){
		int error;
		if(task_pointer+1 == num_tasks){
			float result;
			float base_power = cpu_power[0]*tasks[task_pointer].wcet[0]*0.001;
			for(int freq = 0; freq < 4; freq++){
				// Check for power savings
				if(cpu_power[freq] * tasks[task_pointer].wcet[freq]*0.001 <= base_power){
					task_mask[task_pointer] = freq;
					if(rm_flag) {result = generate_rm_schedule(num_tasks, sys_time, tasks, task_mask, cpu_power, 0);}
					else {result = generate_edf_schedule(num_tasks, sys_time, tasks, task_mask, cpu_power, 0);}
					
					*num_checked = *num_checked + 1;
					if(result != -1 && result < *best_power){
						*best_power = result;
						for(int task = 0; task < num_tasks; task++){best_task_mask[task] = task_mask[task];}
					}
				}
			}
			error = 1;
		}
		else{
			float base_power = cpu_power[0]*tasks[task_pointer].wcet[0]*0.001;
			for(int freq = 0; freq < 4; freq++){
				// Check for power savings
				if(cpu_power[freq] * tasks[task_pointer].wcet[freq]*0.001 <= base_power){
					task_mask[task_pointer] = freq;
					error = generate_ee_schedule_helper(num_tasks, sys_time, tasks, cpu_power, rm_flag, task_pointer+1, task_mask, best_task_mask, best_power, num_checked);
				}
			}
		}
		return error;
}

/**
 * Generates an Energy Efficient Rate Monotonic Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * cpu_power (int[]): CPU Power at Frequency (mW)
 * print_results (int): Flag to print results
 * 
 * Returns:
 * float: Joules cost of schedule
 */
int generate_ee_rm_schedule(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int cpu_power[],
	int print_results) 
{
	printf("Checking 4 frequency values each for %d tasks...\n", num_tasks);
	int task_mask[num_tasks], best_task_mask[num_tasks];
	float best_power_cost = 99999999999;
	float result;
	int combinations_tested = 0;

	for(int task = 0; task < num_tasks; task++){task_mask[task] = 0;}
	int num_checked = 0;
	int error = generate_ee_schedule_helper(num_tasks, sys_time, tasks, cpu_power, 1, 0, task_mask, best_task_mask, &best_power_cost, &num_checked);
	printf("%d iterations checked!\n", num_checked);
	return generate_rm_schedule(num_tasks, sys_time, tasks, best_task_mask, cpu_power, 1);
}

/**
 * Generates an Energy Efficent Earliest Deadline First Schedule for the provided tasks.
 * 
 * Param:
 * num_tasks (int): Number of Tasks
 * sys_time (int): How long of a schedule to create
 * tasks (process_description[]): Task Definitions
 * cpu_power (int[]): CPU Power at Frequency (mW)
 * print_results (int): Flag to print results
 * 
 * Returns:
 * float: Joules cost of schedule
 */
int generate_ee_edf_schedule(
	int num_tasks, 
	int sys_time,
	struct process_description tasks[], 
	int cpu_power[],
	int print_results){
	printf("Checking 4 frequency values each for %d tasks...\n", num_tasks);
	int task_mask[num_tasks], best_task_mask[num_tasks];
	float best_power_cost = 99999999999;
	float result;
	int combinations_tested = 0;

	for(int task = 0; task < num_tasks; task++){task_mask[task] = 0;}
	int num_checked = 0;
	int error = generate_ee_schedule_helper(num_tasks, sys_time, tasks, cpu_power, 0, 0, task_mask, best_task_mask, &best_power_cost, &num_checked);
	printf("%d iterations checked!\n", num_checked);
	return generate_edf_schedule(num_tasks, sys_time, tasks, best_task_mask, cpu_power, 1);
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

	int num_tasks, sys_time, cpu_power[5];

	fscanf(fpointer, "%d %d %d %d %d %d %d",
		&num_tasks, &sys_time, &cpu_power[0], &cpu_power[1], &cpu_power[2], &cpu_power[3], &cpu_power[4]);
	
	printf("Num_tasks: %d over %d seconds.\nCPU Power Costs:\n",num_tasks, sys_time);
	printf("  1188: %4dmW\n  918 : %4dmW\n  648 : %4dmW\n  384 : %4dmW\n  idle: %4dmW\n",
		cpu_power[0], cpu_power[1], cpu_power[2], cpu_power[3], cpu_power[4]);
	
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
			generate_ee_rm_schedule(num_tasks, sys_time, tasks, cpu_power, 1);
		}
		else{
			generate_rm_schedule(num_tasks, sys_time, tasks, task_mask, cpu_power, 1);
		}
	}
	else{
		if(ee){
			generate_ee_edf_schedule(num_tasks, sys_time, tasks, cpu_power, 1);
		}
		else{
			generate_edf_schedule(num_tasks, sys_time, tasks, task_mask, cpu_power, 1);
		}
	}
	return 0;
}