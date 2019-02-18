rm -rf ./results/*

mkdir -p ./results/images

gcc server.c -o server
gcc client.c -o client

num=9

for i in {1..$num}; do;
	zsh runner.sh $i
done;

wait

echo "Calculating statistics"

for i in {1..$num}; do;

	# Filter
	awk '{ if ($0 >= 1 && $0 <= 10000) print $0 }' "./results/client-$i.csv" >  "./results/a-client-$i.csv"
	awk '{ if ($0 >= 1 && $0 <= 10000) print $0 }' "./results/server-$i.csv" >  "./results/a-server-$i.csv"
	rm "./results/client-$i.csv" 
	rm "./results/server-$i.csv"
	mv "./results/a-client-$i.csv" "./results/client-$i.csv"
	mv "./results/a-server-$i.csv" "./results/server-$i.csv"

	# Calculate stats
	awk -F= 'BEGIN { sum = 0; count = 0}       { sum = sum + $0; count = count + 1;} END {print sum/count}' "./results/client-$i.csv"  >> "./results/client-mean.csv"
	awk -F= 'BEGIN { max = -inf }   { if ($0 > max) { max = $0 } } END { print max }'                       "./results/client-$i.csv"  >> "./results/client-max.csv"
	awk -F= 'BEGIN { min =  10000 } { if ($0 < min) { min = $0 } } END { print min }'                       "./results/client-$i.csv"  >> "./results/client-min.csv"

	awk -F= 'BEGIN { sum = 0; count = 0}       { sum = sum + $0; count = count + 1;} END {print sum/count}' "./results/server-$i.csv"  >> "./results/server-mean.csv"
	awk -F= 'BEGIN { max = -inf }   { if ($0 > max) { max = $0 } } END { print max }'                       "./results/server-$i.csv"  >> "./results/server-max.csv"
	awk -F= 'BEGIN { min =  10000 } { if ($0 < min) { min = $0 } } END { print min }'                       "./results/server-$i.csv"  >> "./results/server-min.csv"
done;

echo "Plotting results"

gnuplot -c bm.gp $num