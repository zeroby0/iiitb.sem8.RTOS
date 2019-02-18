# @title: Gnuplot Template
# @author: Aravind Reddy V
# @hints:
#	_{Subscript}, ^{Superscript}
#   1:(2*$2)


#set terminal pngcairo enhanced font "Helvetica,12.0" size 1500,1100
set terminal png size 1500,1100
#set terminal png truecolor             # Set output type to png
set grid                               # Turn on grid

unset log                              # Remove any previous log scaling
unset label                            # Remove any previous labels

set autoscale                          # Scale axes automatically
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically

set datafile separator ","           # Uncomment when using CSV files

# set xtic add(1.1, 2.2)               # Add xtics at points
# set ytic add(1.1, 2.2)               # Add ytics at points

# set xr[0:10]
# set yr[0:10]

# set xtic 24*60*7

set ytic auto

set xlabel "Sample"                 # Set X axis label
set ylabel "Time taken (micro second)"               # Set Y axis label

# Plot individual graphs


do for [t=1:ARG1] {
  outfile = sprintf('./results/images/client-%d.png',t)
  set output outfile
  plotfile = sprintf('./results/client-%d.csv',t)
  plot plotfile with lines title '' lw 2
}

do for [t=1:ARG1] {
  outfile = sprintf('./results/images/server-%d.png',t)
  set output outfile
  plotfile = sprintf('./results/server-%d.csv',t)
  plot plotfile with lines title '' lw 2
}

set ytic auto

set xlabel "number of clients - 1"


# Plot stats

set title 'Maximum time taken at server'
set output './results/images/server-max.png'
plot './results/server-max.csv' with lines title '' lw 2

set title 'Minimum time taken at server'
set output './results/images/server-min.png'
plot './results/server-min.csv' with lines title '' lw 2

set title 'Mean time taken at server'
set output './results/images/server-mean.png'
plot './results/server-mean.csv' with  lines title '' lw 2


set title 'Maximum time taken at client'
set output './results/images/client-max.png'
plot './results/client-max.csv' with lines title '' lw 2

set title 'Minimum time taken at client'
set output './results/images/client-min.png'
plot './results/client-min.csv' with lines title '' lw 2

set  title 'Mean time taken at client'
set output './results/images/client-mean.png'
plot './results/client-mean.csv' with lines title '' lw 2

# plot '/Users/aravind/files/wt.csv' using 1:2 with lines title 'Channel 1'
