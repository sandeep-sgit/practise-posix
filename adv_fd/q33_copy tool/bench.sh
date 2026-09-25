#!/bin/bash

for size in 1048576 4194304 16777216 67108864 268435456 1073741824
do
    echo "===== BUFFER: $size bytes ====="

    /usr/bin/time -f "TIME: %e seconds" \
        ./mycp source.bin copy.bin "$size"

    strace -c -e trace=open,read,write \
        ./mycp source.bin copy.bin "$size"

    echo
done
