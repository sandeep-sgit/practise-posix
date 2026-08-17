#!/bin/bash

for i in 1 2 3 4 5
do
    /usr/bin/time -f "%e" ./mycp source.bin copy.bin 1048576 2>&1 >/dev/null
done | awk '
{
    values[NR] = $1
    sum += $1
}
END {
    n = NR

    # sort
    for (i = 1; i <= n; i++)
        for (j = i + 1; j <= n; j++)
            if (values[i] > values[j]) {
                tmp = values[i]
                values[i] = values[j]
                values[j] = tmp
            }

    if (n % 2 == 1)
        median = values[(n + 1) / 2]
    else
        median = (values[n / 2] + values[n / 2 + 1]) / 2

    printf "count  = %d\n", n
    printf "mean   = %.3f s\n", sum / n
    printf "median = %.3f s\n", median
    printf "min    = %.3f s\n", values[1]
    printf "max    = %.3f s\n", values[n]
    printf "range  = %.3f s\n", values[n] - values[1]
}'
