#!/bin/sh
set -e
# set -x

OUTPUT_FILENAME="pa1_data_$(date '+%Y-%m-%d_%H-%M-%S').csv"
echo "type, target, sum, time, threads" >> $OUTPUT_FILENAME

ITERATION_LIMIT=100

for iteration in $(seq 1 $ITERATION_LIMIT)
do
    echo "$(date '+%Y-%m-%d_%H-%M-%S') Iteration $iteration/$ITERATION_LIMIT"
    for N in 100000000 1000000000 10000000000
    do
        echo "baseline, $N, $(.\/baseline.o $N), 1" >> $OUTPUT_FILENAME
        for THREAD in 1 2 4 8
        do
            for i in multithread multitask
            do
                echo "$i, $N, $(.\/$i.o $N $THREAD), $THREAD" >> $OUTPUT_FILENAME
            done
        done
    done
done
echo "$(date '+%Y-%m-%d_%H-%M-%S') Operation Complete!"