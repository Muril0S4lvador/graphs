#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include "src/algorithms/algorithms.h"
#include "src/adjacency_matrix/matrix.h"

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    graph_print(g);

    graph_destroy(g);

    return 0;
}

/*
valgrind ./main com entradas/a/A-n46-k7.vrp
*/