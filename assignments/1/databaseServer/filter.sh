for i in {1..10}; do;
	awk '{ if ($0 >= 1 && $0 <= 10000) print $0 }' "./results/client-$i.csv"
	# awk -F= 'BEGIN { } { if ($0 > 0) AND ($0 < 10000)}       END {print $0}' "./results/client-$i.csv";
done;