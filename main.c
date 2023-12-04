#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include "src/algorithms/algorithms.h"

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    graph_Clarke_Wright_parallel_route(g);

    Test(g);

    graph_destroy(g);

    return 0;
}