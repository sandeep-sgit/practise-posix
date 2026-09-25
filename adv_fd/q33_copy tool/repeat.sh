#!/bin/bash

for i in 1 2 3 4 5
do
    echo "RUN $i"

    /usr/bin/time -f "TIME=%e" \
        ./mycp source.bin copy.bin 1048576
done
