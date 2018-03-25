CC = gcc
CFLAGS = -Wall -ansi -O3
LDFLAGS = -O3
# OBJ = bin/main.o bin/load_graph.o bin/graph_common.o bin/soft_strategy.o
OBJ = bin/main.o bin/load_graph.o bin/union_find.o bin/tree.o bin/individual.o bin/rand_perm.o bin/genetic.o bin/touch_reduce.o bin/graph.o

all: clean st-heuristic

st-heuristic: $(OBJ) makefile
	gcc -o st-heuristic $(OBJ) $(LDFLAGS)

bin/main.o: src/main.c include/load_graph.h makefile
	gcc -c -o bin/main.o src/main.c $(CFLAGS)

bin/graph.o: src/graph.c include/graph.h makefile
	gcc -c -o bin/graph.o src/graph.c $(CFLAGS)

bin/load_graph.o: src/load_graph.c include/load_graph.h include/graph.h makefile
	gcc -c -o bin/load_graph.o src/load_graph.c $(CFLAGS)

bin/union_find.o: src/union_find.c include/union_find.h makefile
	gcc -c -o bin/union_find.o src/union_find.c $(CFLAGS)

bin/tree.o: src/tree.c include/tree.h makefile
	gcc -c -o bin/tree.o src/tree.c $(CFLAGS)

bin/rand_perm.o: src/rand_perm.c include/rand_perm.h makefile
	gcc -c -o bin/rand_perm.o src/rand_perm.c $(CFLAGS)

bin/individual.o: src/individual.c include/individual.h include/tree.h include/graph.h include/union_find.h  makefile
	gcc -c -o bin/individual.o src/individual.c $(CFLAGS)

bin/genetic.o: src/genetic.c include/genetic.h include/individual.h makefile
	gcc -c -o bin/genetic.o src/genetic.c $(CFLAGS)

bin/touch_reduce.o: src/touch_reduce.c include/touch_reduce.h makefile
	gcc -c -o bin/touch_reduce.o src/touch_reduce.c $(CFLAGS)

clean:
	rm -rf st-heuristic
	rm -rf bin/*.o
	rm -rf *~
	rm -rf */*~

rebuild:
	make clean
	make
