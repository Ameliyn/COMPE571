# COMPE571 Programming Assignment 1

Prepared by Skye Russ

### Hardware Specs:

- AMD Ryzen™ 5 5600H with Radeon™ Graphics × 12
- NVIDIA GeForce RTX™ 3060 Laptop GPU
- 16GB Ram

## CASE: Baseline

This case simply adds the numbers from `[1,N)` in a simple for loop.
Run this case by executing `./baseline.o N` where N is an integer you wish to sum to.

## CASE: Multithreading

This case simply adds the numbers from `[1,N)` across `NUM_THREADS` threads..
Run this case by executing `./multithread.o N NUM_THREADS` where N is an integer you wish to sum to and NUM_THREADS is an integer number of threads.

## CASE: Multitasking (Fork)

This case adds the numbers from `[1,N)` across `NUM_TASKS` that are connected with Pipes. 
Run this case using `./multitask_fork.o N NUM_TASKS` where N is an integer you wish to sum to and NUM_TASKS is an integer number of tasks.

## CASE: Multitasking (Popen)

This case adds the numbers from `[1,N)` across `NUM_TASKS` that are opened using `popen()`.
Run this case using `./multitask_popen.o N_LOWER_BOUND N_UPPER_BOUND NUM_TASKS PARENT_FLAG` where N_LOWER is the first integer to sum, N_UPPER is an integer you wish to sum to, NUM_TASKS is an integer number of tasks, and PARENT_FLAG is to indicate this is the parent process (when running this manually, that should always be set to 1). (To add the numbers `[1,N)`, an example execution would be `./multitask_popen.o 1 N NUM_TASKS 1`).

Note: The command line execution of this script is a little unclean because it creates child processes that calculate a sum using the same executable. In order to allow this script to be run with 1 child, the parent flag is included.

## DATA COLLECTION

There is an additional folder of data collection scripts. The base c files are exactly the same as the main folder's files, however they only output the calculated sum and time taken instead of additional text. The provided `./collect_data.sh` bash script runs all four cases 100 times using 1, 4, and 8 threads/tasks for N values 100000000, 1000000000, 10000000000 and outputs the results to a CSV file. One CSV file of 100 iterations is included in the repository. On the provided hardware specs, this bash script took ~1 minute per iteration (~100 minutes total).

## docs

In the docs folder are all graphs and images used in the report as well as a PDF of the report.