# Gnuplot script for plotting data in file "green.dat"
set terminal png
set output "green.png"

set title "Execution time to perform 20k productions with 2 green threads"

set key right center

set xlabel "Number of productions"
set ylabel "Time (ms)"

set xrange [0:20000]
set yrange [0:180]

plot "green.dat" u 1:2 w linespoints title ""