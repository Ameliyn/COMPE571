# COMPUTER ENGINEERING 571: EMBEDDED OPERATING SYSTEMS PROGRAMMING ASSIGNMENT 3

## How to use this repository

1. Compile by executing `gcc dvfs.c -o dvfs.o`
2. Execute using `./dvfs.o INPUT_FILENAME.txt SCHEDULING_ALGORITHM`
    Example: `./dvfs.o input1.txt RM EE`

## Implementation

dvfs.c implements Rate Monotonic and Earliest Deadline First scheduling algorithms with an optional flag to minimize the power usage. The script inputs a text document that contains the number of tasks, CPU Frequency Costs (in milliwatts), and each task worst case execution time at different CPU frequencies.