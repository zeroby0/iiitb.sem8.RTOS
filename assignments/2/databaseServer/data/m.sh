for i in {1..200}; do;
	echo "This-is-file-$i.Now-all-I'm-doing-is-increasing-letter-count.This-repetition-compresses-nicely." > "$i.txt"
	wc "$i.txt"
done;