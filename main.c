#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main(){

    Graph *g = graph_read_file_CVRPLIB();
    /*
    
    */
    graph_Clarke_Wright_route(g);

    img_print_route(g, "imgs/routes.dot");

    graph_destroy(g);

    return 0;
}