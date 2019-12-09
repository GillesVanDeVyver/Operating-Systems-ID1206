# Gnuplot script for plotting data in file "multi.dat"
set terminal png
set output "multi.png"

set title "Execution time to perform 20.000 productions with 2 threads "

set key right center

set xlabel "Number of productions"
set ylabel "Time (ms)"

set xrange [0:20000]
set yrange [0:180]

set key right bottom
set key invert

plot 'pthread.dat' with lines title "Pthreads", 'green.dat' with linespoints title "Green"
