#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    // graph_Clarke_Wright_serial_route(g);

    graph_Clarke_Wright_parallel_route(g);

    img_print_route(g, "imgs/Route_Bef.dot");

    graph_Variable_Neighborhood_Search(g);

    img_print_route(g, "imgs/Route_Aft.dot");

    printf("%.3f\n", route_return_total_cost(g));
    printf("%.0f\n", route_return_optimal_cost(g));

    graph_destroy(g);

    return 0;
}