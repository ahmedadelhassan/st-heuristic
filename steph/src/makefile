CC = gcc
#CFLAGS = -Wall -ansi -pg
CFLAGS = -Wall -O3
LDFLAGS = -pg
OBJ = obj/array_utils.o obj/color.o obj/edge.o obj/graph.o obj/individual.o obj/individual_heap.o obj/list.o obj/node.o obj/optimizer.o obj/random.o

st-heuristic: $(CFLAGS) $(OBJ)
	gcc -o st-heuristic $(OBJ) $(LDFLAGS)

clean:
	rm -rf st-heuristic
	rm -rf obj/*.o
	rm -rf *~
	rm -rf */*~

rebuild:
	make clean
	make
