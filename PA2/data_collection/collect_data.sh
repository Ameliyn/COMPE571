#!/bin/sh
set -e
set -x

OUTPUT_FILENAME="pa2_data_$(date '+%Y-%m-%d_%H-%M-%S').csv"
echo "Type, Quantum, Average Response Time, Context Switch Time" >> $OUTPUT_FILENAME

ITERATION_LIMIT=100
QUANTUM_LIMIT=250000

for iteration in $(seq 1 $ITERATION_LIMIT)
do
    echo "$(date '+%Y-%m-%d_%H-%M-%S') Iteration $iteration/$ITERATION_LIMIT"
    for basic in fcfs sjf
    do
        echo "$basic, $(.\/$basic.o)" >> $OUTPUT_FILENAME
    done
    for N in $(seq 0 5000 $QUANTUM_LIMIT)
    do
        for i in mlfq rr
        do
            echo "$i, $(.\/$i.o $N)" >> $OUTPUT_FILENAME
        done
    done
done
echo "$(date '+%Y-%m-%d_%H-%M-%S') Operation Complete!"