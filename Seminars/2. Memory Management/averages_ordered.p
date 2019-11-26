

# Gnuplot script for plotting data in file "averages_ordered.dat"
set terminal png
set output "averages_ordered.png"

set title "Evaluation of length of ordered free list"

set key right center

set xlabel "Number of allocations"
set ylabel "Length of free list"

set xrange [0:10000]
set yrange [0:85]

plot "averages_ordered.dat" u 1:2 w linespoints title "averages_ordered"