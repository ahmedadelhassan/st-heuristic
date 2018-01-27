CC = gcc
CFLAGS = -Wall -ansi -O2
LDFLAGS =
OBJ = bin/main.o bin/load_graph.o bin/graph_common.o

st-heuristic: $(OBJ)
	gcc -o st-heuristic $(OBJ) $(LDFLAGS)

bin/main.o: src/main.c include/load_graph.h
	gcc -c -o bin/main.o src/main.c $(CFLAGS)

bin/load_graph.o: src/load_graph.c include/load_graph.h include/graph_common.h
	gcc -c -o bin/load_graph.o src/load_graph.c $(CFLAGS)

bin/graph_common.o: src/graph_common.c include/graph_common.h
	gcc -c -o bin/graph_common.o src/graph_common.c $(CFLAGS)

clean:
	rm -rf st-heuristic
	rm -rf bin/*.o
	rm -rf *~
	rm -rf */*~

rebuild:
	make clean
	make
