# Pass as argument 1 the number of simultaneous clients.


# server: Second argument is the file to which
# results should be written
# 
# First argument is used to create server key
./server "$1" "./results/server-$1.csv" & server_pid=$!

NUMBER_OF_RUNS=500

echo "N = $1"
for i in {1..$NUMBER_OF_RUNS}; do; # 500 runs
	for j in {1..$1}; do; # $1 clients
		# First is to be used for
		# creating server queue key
		# 
		# Second argument is the file that
		# will be requested
		./client "$1" "$j"  >> "./results/client-$1.csv" & pids+=($!)
	done;
	wait "${pids[@]}"
	pids=()

done;
sleep 2
kill $server_pid
echo "done $1"

# echo "N = 2"
# echo "./server >> 1s.csv"
# echo "Running clients"
# for i in {1..100}; do;
# 	./client 10 >> 1c.csv
# 	wait
# done;

# wait
