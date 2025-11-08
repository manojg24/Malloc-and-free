Malloc and free functions without the libraries.


Performance Summary<br>
Metric&nbsp;&nbsp;&nbsp;&nbsp;Result (example)&nbsp;&nbsp;&nbsp;&nbsp;Description<br>
Heap size&nbsp;&nbsp;&nbsp;&nbsp;64 KB&nbsp;&nbsp;&nbsp;&nbsp;Static region managed manually<br>
Allocation latency&nbsp;&nbsp;&nbsp;&nbsp;~1.4 µs per alloc/free&nbsp;&nbsp;&nbsp;&nbsp;Average over 10k ops<br>
Peak fragmentation&nbsp;&nbsp;&nbsp;&nbsp; ~33%	&nbsp;&nbsp;&nbsp;&nbsp;  Simulated load<br>
Memory utilization&nbsp;&nbsp;&nbsp;&nbsp; ~82%&nbsp;&nbsp;&nbsp;&nbsp;  After stress test<br>


Run command: gcc -std=c11 -Wall malloc_free.c -o malloc_free <br>
Output Viewer: ./malloc_free > heap_output.txt
