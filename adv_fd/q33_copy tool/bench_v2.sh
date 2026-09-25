#!/bin/bash

echo "buffer,time"

for size in 1024 4096 16384 65536 262144 1048576 4194304 16777216 67108864 268435456 1073741824
do
    time=$(
        /usr/bin/time -f "%e" \
        ./mycp my200.bin copy2.bin "$size" \
        2>&1 >/dev/null
    )

    echo "$size,$time"
done
