#!/bin/bash
START=${1:-1}
INC=${2:-1}
FINISH=${3:-8}
N=${4:-66}
TRIALS=1
echo "Testing with threading in range $START-$FINISH in increments of $INC for $N by $N array."
OUTPUT="time.log"
>$OUTPUT
for i in `seq $START $INC $FINISH`; do
  for r in `seq 1 ${TRIALS}`; do
    (time mpirun -n $i relax -n $N)  2>&1 > /dev/null | grep real | awk '{print $2}'|sed "s/^[0-9]\+m//" | xargs printf "$i\t%s\n" >> $OUTPUT
  done
done

gnuplot <<- EOF
    set xlabel "Threads"
    set ylabel "Time (s)"
    #set term dumb
    set term png
    set output "${OUTPUT}.png"
    plot [$START:$FINISH] "${OUTPUT}" title "Execution Time (real)" with lines
EOF

S=$(head -1 ${OUTPUT} | awk '{print $1}')
ST=$(grep "^$S[^0-9]" ${OUTPUT} | awk '{ sum += $2; n++ } END { if (n > 0) print sum / n; }')
echo "T(1) = ${ST}"
gnuplot <<- EOF
    set xlabel "Threads"
    set ylabel "Speedup"
    set title "Speedup Results"
    set xrange [$START:$FINISH]
    #set term dumb
    set term png
    set output "speedup.png"
    plot "${OUTPUT}" using 1:($ST/\$2) title "(real)" smooth unique lw 3
EOF
