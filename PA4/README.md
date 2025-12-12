# COMPE571 Embedded Operating Systems Programming Assignment 4: Filesystems

## File Descriptions

filesystem.c -- Main C File
filesystem.o -- Precompiled C File (compiled and tested on Ubuntu 24.04)
data1.txt -- Example Disk Operations 1
data2.txt -- Example Disk Operations 2
README.md -- YOU ARE HERE
COMPE571PA3SkyeRussReport.pdf -- Writeup Report

## How to use this repository (Compilation and Execution)

1. Compile by executing `gcc filesystem.c -o filesystem.o`
2. Execute using `./dvfs.o INPUT_FILENAME.txt PAGE_REPLACEMENT_ALGORITHM`
    Example: `./dvfs.o data1.txt LRU`
    NOTE: All available algorithms (RAND, FIFO, LRU, PER, FUT)

## filesystem.c Implementation Notes

The main function takes in command line inputs for the input file and the page replacement algorithms. Available page replacement algorithms are: Random (RAND), First In First Out (FIFO), Least Recently Used (LRU), Periodic Reference Reset (PER), and Future Prediction (FUT). The script initializes an array of `main_memory_pages` that store the process_number, virtual page number, offset, dirty_bit, reference_bit, and last_used_time_unit. These values are used for page replacement decisions.

The function then goes into the main control loop where it reads one line of the file, checks if it has the corresponding page loaded, and replaces the page as needed. The replacement is decided by a number of helper functions, with each replacement algorithm having a corresponding helper function. Once the page to replace is selected, the function checks whether the page to be replaced is dirty and writes it back to the main memory as necessary.

Once all operations have been completed, the program outputs the number of operations completed, number of page faults, number of page hits, number of disk references, and number of dirty page writes. Below is the output from running both data1.txt and data2.txt for all scheduling algorithms.

## Data Tables and Results

### data1.txt
|  Algorithm          | Page Faults         | Disk Accesses      | Dirty Page Writes   |
| :-----------------: | :-----------------: | :-----------------:| :-----------------: |
| FIFO                | 3248                | 6309               | 3061                |
| RAND                | 3552                | 6879               | 3327                |
| LRU                 | 3268                | 6328               | 3060                |
| PER                 | 3629                | 6951               | 3322                |
| FUT                 | 3057                | 5940               | 2883                |

### data2.txt
|  Algorithm          | Page Faults         | Disk Accesses      | Dirty Page Writes   |
| :-----------------: | :-----------------: | :-----------------:| :-----------------: |
| FIFO                | 2878                | 5625               | 2747                |
| RAND                | 3202                | 6216               | 3014                |
| LRU                 | 2876                | 5596               | 2720                |
| PER                 | 3279                | 6318               | 3039                |
| FUT                 | 2714                | 5297               | 2583                |