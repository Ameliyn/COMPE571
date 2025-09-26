#!/bin/bash
set -e
set -x

OUTPUT_FILENAME="baseline_$(date '+%Y-%m-%d_%H-%M-%S').csv"
echo "target, sum, time" >> $OUTPUT_FILENAME

for n in 100000000 1000000000 10000000000
do
    for i in $(seq 1 100)
    do
        echo "$n, $(./baseline.o $n)\n" >> $OUTPUT_FILENAME
    done
done