# COMPUTER ENGINEERING 571: EMBEDDED OPERATING SYSTEMS PROGRAMMING ASSIGNMENT 3

## How to use

1. Compile by executing `gcc dvfs.c -lm -o dvfs.o`
2. Execute using `./dvfs.o INPUT_FILENAME.txt SCHEDULING_ALGORITHM`
    Example: `./dvfs.o input1.txt RM EE`

## Implementation

dvfs.c implements Rate Monotonic and Earliest Deadline First scheduling algorithms with an optional flag to minimize the power usage based on hard coded CPU frequency costs.