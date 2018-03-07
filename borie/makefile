CC = gcc
CFLAGS = -Wall -ansi -pg
LDFLAGS = -pg
# OBJ = bin/main.o bin/load_graph.o bin/graph_common.o bin/soft_strategy.o
OBJ = bin/main.o bin/load_graph.o bin/soft_strategy.o

st-heuristic: $(OBJ)
	gcc -o st-heuristic $(OBJ) $(LDFLAGS)

bin/main.o: src/main.c include/load_graph.h
	gcc -c -o bin/main.o src/main.c $(CFLAGS)

bin/load_graph.o: src/load_graph.c include/load_graph.h include/graph_common.h
	gcc -c -o bin/load_graph.o src/load_graph.c $(CFLAGS)

bin/graph_common.o: src/graph_common.c include/graph_common.h
	gcc -c -o bin/graph_common.o src/graph_common.c $(CFLAGS)

bin/soft_strategy.o: src/soft_strategy.c include/soft_strategy.h
	gcc -c -o bin/soft_strategy.o src/soft_strategy.c $(CFLAGS)

clean:
	rm -rf st-heuristic
	rm -rf bin/*.o
	rm -rf *~
	rm -rf */*~

rebuild:
	make clean
	make
