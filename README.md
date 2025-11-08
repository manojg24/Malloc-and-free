Malloc and free functions without the libraries.


Performance Summary<br>
Metric<tb><tb><tb><tb><tb>Result (example)<tb><tb><tb><tb><tb>Description<br>
Heap size<tb><tb><tb><tb><tb>64 KB<tb><tb><tb><tb><tb>Static region managed manually<br>
Allocation latency<tb><tb><tb><tb><tb>~1.4 µs per alloc/free<tb><tb><tb><tb><tb>Average over 10k ops<br>
Peak fragmentation<tb><tb><tb><tb><tb> ~33%	<tb><tb><tb><tb><tb>  Simulated load<br>
Memory utilization<tb><tb><tb><tb><tb> ~82%<tb><tb><tb><tb><tb>  After stress test<br>


Run command: gcc -std=c11 -Wall malloc_free.c -o malloc_free <br>
Output Viewer: ./malloc_free > heap_output.txt
