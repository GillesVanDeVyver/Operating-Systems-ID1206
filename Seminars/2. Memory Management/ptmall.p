
# Gnuplot script for plotting data in file "ptmall.dat"
set terminal png
set output "ptmall.png"

set title "First implementation of dalloc()"

set key right center

set xlabel "Buffer size"
set ylabel "Time per dalloc (ms)"

set xrange [0:1000]
set yrange [0:0.3]

plot "ptmall.dat" u 1:2 w linespoints title "ptmall"