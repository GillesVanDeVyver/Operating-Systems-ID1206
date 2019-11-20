# Gnuplot script for plotting data in file "clock.dat"
set terminal png
set output "clock.png"

set title "Page replacement using different policies"

set key right center

set xlabel "frames in memory"
set ylabel "hit ratio"

set xrange [0:100]
set yrange [0:1]

plot "clock.dat" u 1:2 w linespoints title "clock"