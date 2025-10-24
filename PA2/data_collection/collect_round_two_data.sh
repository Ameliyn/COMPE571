#!/bin/sh
set -e
set -x

OUTPUT_FILENAME="pa2_data_$(date '+%Y-%m-%d_%H-%M-%S').csv"
echo "Type, Quantum, Average Response Time, Context Switch Time, Second Queue Usage" >> $OUTPUT_FILENAME

ITERATION_LIMIT=100
QUANTUM_LIMIT=

for iteration in $(seq 1 $ITERATION_LIMIT)
do
    echo "$(date '+%Y-%m-%d_%H-%M-%S') Iteration $iteration/$ITERATION_LIMIT"
    for N in $(seq 0 500 20000)
    do
        echo "rr, $(.\/rr.o $N), 0" >> $OUTPUT_FILENAME
    done
    for N in $(seq 15000 1000 32000)
    do
        echo "mlfq, $(.\/mlfq.o $N)" >> $OUTPUT_FILENAME
    done
    for N in $(seq 81000 1000 100000)
    do
        echo "mlfq, $(.\/mlfq.o $N)" >> $OUTPUT_FILENAME
    done
done
echo "$(date '+%Y-%m-%d_%H-%M-%S') Operation Complete!"