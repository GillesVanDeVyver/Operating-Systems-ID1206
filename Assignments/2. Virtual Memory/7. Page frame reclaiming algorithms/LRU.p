# Gnuplot script for plotting data in file "lru.dat"
set terminal png
set output "lru.png"

set title "Page replacement using different policies"

set key right center

set xlabel "frames in memory"
set ylabel "hit ratio"

set xrange [0:100]
set yrange [0:1]

plot "lru.dat" u 1:2 w linespoints title "Last recently used"