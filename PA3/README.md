# COMPUTER ENGINEERING 571: EMBEDDED OPERATING SYSTEMS PROGRAMMING ASSIGNMENT 3

## File Descriptions

dvfs.c -- Main C File
dvfs.o -- Precompiled C File (compiled and tested on Ubuntu 24.04)
input1.txt -- Example task list 1
input2.txt -- Example task list 2
README.md -- YOU ARE HERE
edf_out.txt -- Example output from executing `./dvfs.o input1.txt EDF EE >> edf_out.txt`
edf_ee_out.txt -- Example output from executing `./dvfs.o input1.txt EDF EE >> edf_ee_out.txt`
rm_out.txt -- Example output from executing `./dvfs.o input1.txt EDF EE >> rm_out.txt`
rm_ee_out.txt -- Example output from executing `./dvfs.o input1.txt EDF EE >> rm_ee_out.txt`
PA3_REPORT_SKYERUSS.pdf -- Writeup Report

## How to use this repository (Compilation and Execution)

1. Compile by executing `gcc dvfs.c -o dvfs.o`
2. Execute using `./dvfs.o INPUT_FILENAME.txt SCHEDULING_ALGORITHM`
    Example: `./dvfs.o input1.txt RM EE`

## dvfs.c Implementation Notes

dvfs.c implements Rate Monotonic and Earliest Deadline First scheduling algorithms with an optional flag to minimize the power usage. The script inputs a text document that contains the number of tasks, CPU Frequency Costs (in milliwatts), and each task worst case execution time at different CPU frequencies.

The Rate Monotonic implementation takes in a task list and frequency and attempts to use Rate Monotonic Scheduling to schedule all the tasks for the given amount of time. Before scheduling, the function calculates the CPU Utilization of all tasks combined and ensures it is < 1.

The Earliest Deadline First implementation takes in a task list and frequency and attempts to use Earliest Deadline First Scheduling to schedule all the tasks for the given amount of time. Before scheduling, the function calculates the CPU Utilization of all tasks combined and ensures it is < 1.

To make either of these cases Energy Efficient, the script uses a recursive helper method to iterate through all frequencies for all tasks in the set. Before attempting to calculate a schedule, the helper method checks that the slower CPU frequency will result in an energy saving as compared to maximum CPU frequency.