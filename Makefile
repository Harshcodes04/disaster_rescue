CC = gcc
CFLAGS = -Wall

all: rescue

rescue: main.c dispatch.c heap.c queue.c hash.c graph.c render.c
	$(CC) $(CFLAGS) main.c dispatch.c heap.c queue.c hash.c graph.c render.c -o rescue

tests: test_queue test_heap test_hash test_render test_graph

test_queue: test_queue.c queue.c
	$(CC) $(CFLAGS) test_queue.c queue.c -o test_queue

test_heap: test_heap.c heap.c
	$(CC) $(CFLAGS) test_heap.c heap.c -o test_heap

test_hash: test_hash.c hash.c
	$(CC) $(CFLAGS) test_hash.c hash.c -o test_hash

test_render: test_render.c render.c graph.c heap.c
	$(CC) $(CFLAGS) test_render.c render.c graph.c heap.c -o test_render

test_graph: test_graph.c graph.c heap.c
	$(CC) $(CFLAGS) test_graph.c graph.c heap.c -o test_graph

clean:
	rm -f rescue test_queue test_heap test_hash test_render test_graph *.exe
