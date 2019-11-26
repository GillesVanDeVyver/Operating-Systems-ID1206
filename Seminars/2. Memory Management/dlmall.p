
# Gnuplot script for plotting data in file "dlmall.dat"
set terminal png
set output "dlmall.png"

set title "dalloc() with unordered free list"

set key right center

set xlabel "Number of allocations performed"
set ylabel "Time per dalloc (ms)"

set xrange [0:1000]
set yrange [0:0.3]

plot "dlmall.dat" u 1:2 w linespoints title "dlmall"