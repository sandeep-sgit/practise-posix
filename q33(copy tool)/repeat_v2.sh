#!/bin/bash

for i in 1 2 3 4 5
do
    /usr/bin/time -f "%e" ./mycp source.bin copy.bin 1048576 2>&1 >/dev/null
done
