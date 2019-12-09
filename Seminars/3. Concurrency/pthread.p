# Gnuplot script for plotting data in file "pthread.dat"
set terminal png
set output "pthread.png"

set title "Execution time to perform 20k productions with 2 Pthreads"

set key right center

set xlabel "Number of productions"
set ylabel "Time (ms)"

set xrange [0:20000]
set yrange [0:180]

plot "pthread.dat" u 1:2 w linespoints title ""