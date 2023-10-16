#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main(){

    Graph *g = graph_read_file_CVRPLIB();

    graph_Clarke_Wright_route(g);

    img_print_route(g, "imgs/routes.dot");

    graph_destroy(g);

    return 0;
}

/*
Loops:
    A-n39-k6.vrp
    A-n45-k6.vrp
    A-n61-k9.vrp
    A-n63-k9.vrp
    A-n65-k9.vrp
*/