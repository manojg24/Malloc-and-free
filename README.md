Malloc and free functions without the libraries.


Performance Summary<br>
Metric	                  Result (example)	                  Description<br>
Heap size	                64 KB	                              Static region managed manually<br>
Allocation latency	      ~1.4 µs per alloc/free	            Average over 10k ops<br>
Peak fragmentation	      ~33%	                              Simulated load<br>
Memory utilization	      ~82%	                              After stress test<br>


Run command: gcc -std=c11 -Wall malloc_free.c -o malloc_free <br>
Output Viewer: ./malloc_free > heap_output.txt
