
# Gnuplot script for plotting data in file "flist.dat"
set terminal png
set output "flist.png"

set title "Length of list with free blocks - Ordered"

set key right center

set xlabel "Number of allocations performed"
set ylabel "Number of free blocks"

set xrange [0:1000]
set yrange [0:15]

plot "flist.dat" u 1:2 w linespoints title "flist"