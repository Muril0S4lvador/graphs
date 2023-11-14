#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g);

    route_print(g);
    img_print_route(g, "imgs/route.dot");

    graph_destroy(g);

    return 0;
}