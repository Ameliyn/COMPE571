#!/bin/sh
set -e
set -x

OUTPUT_FILENAME="pa2_data_$(date '+%Y-%m-%d_%H-%M-%S').csv"
echo "Type, Quantum, Average Response Time, Context Switch Time, Queue Usage" >> $OUTPUT_FILENAME

ITERATION_LIMIT=100

for iteration in $(seq 1 $ITERATION_LIMIT)
do
    echo "$(date '+%Y-%m-%d_%H-%M-%S') Iteration $iteration/$ITERATION_LIMIT"
    for basic in fcfs sjf
    do
        echo "$basic, $(.\/$basic.o), 0" >> $OUTPUT_FILENAME
    done
    echo "mlfq, $(.\/mlfq.o 84000)" >> $OUTPUT_FILENAME
    echo "rr, $(.\/rr.o 17000), 0" >> $OUTPUT_FILENAME    
done
echo "$(date '+%Y-%m-%d_%H-%M-%S') Operation Complete!"