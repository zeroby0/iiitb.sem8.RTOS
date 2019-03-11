max_simultaneous_clients=16
runs_per_sim=10000

rm -rf ./results/*
mkdir -p ./results/images

gcc server.c -o server
gcc client.c -o client


for i in {1..$max_simultaneous_clients}; do;
	for j in {1..$runs_per_sim}; do;
		./server & server_pid=$!
		echo "$i $j";
		for k in {1..$i}; do;
			./client >> "./results/client-$i.csv" 2>> /dev/null & pids+=($!)
		done;
		wait "${pids[@]}"
		pids=()
		kill $server_pid
	done;
done;


for i in {1..$max_simultaneous_clients}; do;

	# Calculate stats
	awk -F= 'BEGIN { sum = 0; count = 0}       { sum = sum + $0; count = count + 1;} END {print sum/count}' "./results/client-$i.csv"  >> "./results/client-mean.csv"
	awk -F= 'BEGIN { max = -inf }   { if ($0 > max) { max = $0 } } END { print max }'                       "./results/client-$i.csv"  >> "./results/client-max.csv"
	awk -F= 'BEGIN { min =  10000 } { if ($0 < min) { min = $0 } } END { print min }'                       "./results/client-$i.csv"  >> "./results/client-min.csv"

done;


gnuplot -c bm.gp $max_simultaneous_clients



