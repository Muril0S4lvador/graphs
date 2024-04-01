all: objects objects/main.o main

teste: objects
	gcc -g -o main $(ARG1) objects/graph.o objects/graphviz_print.o objects/algorithms.o objects/union_find.o objects/vector.o objects/matrix.o objects/data.o -lm

objects: objects/graph.o objects/graphviz_print.o objects/algorithms.o objects/union_find.o objects/vector.o objects/matrix.o objects/data.o

objects/data.o: src/data/data.h src/data/data.c
	gcc -g -c src/data/data.c -Wall -o objects/data.o

objects/matrix.o: src/adjacency_matrix/matrix.h src/adjacency_matrix/matrix.c
	gcc -g -c src/adjacency_matrix/matrix.c -Wall -o objects/matrix.o

objects/vector.o: src/Vector/vector.h src/Vector/vector.c
	gcc -g -c src/Vector/vector.c -Wall -o objects/vector.o

objects/union_find.o: src/union_find/union_find.h src/union_find/union_find.c
	gcc -g -c src/union_find/union_find.c -Wall -o objects/union_find.o

objects/graphviz_print.o: src/graphviz_print/graphviz_print.h src/graphviz_print/graphviz_print.c
	gcc -g -c src/graphviz_print/graphviz_print.c -Wall -o objects/graphviz_print.o

objects/algorithms.o: src/algorithms/algorithms.h src/algorithms/algorithms.c
	gcc -g -c src/algorithms/algorithms.c -Wall -o objects/algorithms.o

objects/graph.o: src/graph/graph.h src/graph/graph.c
	gcc -g -c src/graph/graph.c -Wall -o objects/graph.o

objects/main.o: main.c
	gcc -g -c main.c -Wall -o objects/main.o

main: objects/main.o objects/graph.o objects/graphviz_print.o objects/algorithms.o objects/union_find.o objects/vector.o objects/matrix.o objects/data.o
	gcc -g -o main objects/main.o objects/graph.o objects/graphviz_print.o objects/algorithms.o objects/union_find.o objects/vector.o objects/matrix.o objects/data.o -lm

imgs_clean:
	rm -r imgs/*.dot* imgs/arq_dots/*

clean:
	rm -rf *.o *~ main