README FILE FOR MY PROGRAM

TO RUN: ./myhie -i data/{filename, e.g 2batch-10000.csv} -k {sorterNum} -a {attribute to sort. please use 1/4/5/6} -o {order:a/d, a for ASEC, d for DESC} -s {output file name}

PLEASE NOTE: for the attribute field, please use 1/4/5/6, instead of 0/3/4/5. I made this design choice since it is more intuitive

When testing on large file, the sorting process could run very slow. But it works! It is nice to have more sorters when having big file, say 20. Running the 3batch-100000.csv on abudhabi server takes averagely 900s for me. But running small file can be fast.

Thank you!
