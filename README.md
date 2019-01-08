# Dijkstra
Calculate all source shortest path problem by applying Dijkstran=|V|times with both serial and OpenMP.
In serial.c and OpenMP.c, I wrote a void printArr(int dist[]) function to print the result of shortest paths for each source and each vertexs.
I call the function in line 174(serial.c), and line 177(OpenMP.c).
You can choose to use this function when you are testing tiny graph(tiny.txt) to test the correctness.
And choose not to call this function when testing test_graph_small.txt and test_graph.txt, because printing is very time consuming.

Also, you can set the num_of_thread for OpenMP.c in line 225.

Mine OpenMp version is slower than serial one, because I only create one minHeap for serial one, and keep refreshing it for each vertex source.
For OpenMp, I create minHeap for each vertex source to avoid crashing, so it is slower.

Timing:
For serial.c:
 test_graph_small.txt: 4.410999s
 test_graph.txt: 5319.476639s

For OpenMP.c:
test_graph_small.txt:
8.666312s when number_of_threads = 3
7.423817s when number_of_threads = 5
7.758672s when number_of_threads = 10
9.605392s when number_of_threads = 100
