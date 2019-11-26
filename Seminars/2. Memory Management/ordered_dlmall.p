
# Gnuplot script for plotting data in file "ordered_dlmall.dat"
set terminal png
set output "ordered_dlmall.png"

set title "dalloc() with ordered free list"

set key right center

set xlabel "Number of allocations performed"
set ylabel "Time per dalloc (ms)"

set xrange [0:1000]
set yrange [0:0.3]

plot "ordered_dlmall.dat" u 1:2 w linespoints title "ordered_dlmall"