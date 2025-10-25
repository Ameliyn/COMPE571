\# Computer Engineering 571: Embedded Operating Systems



Prepared by Skye Russ



\## How to use this repository



The base level folder includes four C files that implement First Come First Serve, Round Robin, Shortest Job First, and Multi-Level-Feedback-Queue scheduling algorithms. 

To change the workload size, change the defined values at the top of the desired algorithm's C file.



\## File Description



first\_come\_first\_serve.c/o -- First Come First Serve Algorithm

multi\_level\_feedback\_queue.c/o -- Multi Level Feedback Queue Algorithm

round\_robin.c/o -- Round Robin Algorithm

shortest\_job\_first.c/o -- Shortest Job First Algorithm

data\_collection/collect\_data.sh -- Bash script to collect data.

data\_collection/create\_graphs.m -- MATLAB script to create graphs and tables for data processing.

data\_collection/\*.csv -- CSV files of collected data.



\## Compilation



Execute `gcc desired\_case.c` to compile each case. This has already been completed for all cases which generated the `.o` files.



\## Data Collection



The Data Collection folder includes identical C files to the main folder, however they only output the experimental results for easy allocation into a CSV file. The provided bash script runs 100 iterations of each scheduling algorithm for a number of time quantums.



