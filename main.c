#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/info/info.h"
#include "src/graphviz_print/graphviz_print.h"

#include <time.h>

void distanceToOptimal(double cost, double optimal){
    double difference;
    difference = (double)((cost - optimal) / optimal) * 100;
    printf("%.0lf %.0lf (%.2lf%%)\n", cost, optimal, difference);
}

int main( int argc, char* argv[] ){
    int seed = 1;

    srand(seed);

    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    info_construct(g, seed);

    graph_Clarke_Wright_parallel_route(g);
    graph_enables_routes(g);

    graph_Variable_Neighborhood_Search(g);

    info_set_routes(graph_return_route(g));

    info_print();

    graph_destroy(g);
    info_destroy();

    return 0;
}