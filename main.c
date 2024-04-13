#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include <time.h>

void distanceToOptimal(double cost, double optimal){
    double difference;
    difference = (double)((cost - optimal) / optimal) * 100;
    printf("%.0lf %.0lf (%.2lf%%)\n", cost, optimal, difference);
}

int main( int argc, char* argv[] ){

    srand(1);


    clock_t end, start = clock();
    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    // Prepara solução inicial para ser otimizada
    graph_Clarke_Wright_parallel_route(g);
    graph_enables_routes(g);
    graph_2opt(g);

    graph_Variable_Neighborhood_Search(g);

    end = clock();

    distanceToOptimal(graph_return_total_cost(g), graph_return_optimal_cost(g));

    printf("Time: %.0lf ms\n", ((double)(end - start) / CLOCKS_PER_SEC) * 1000);

    route_print(g);

    graph_destroy(g);

    return 0;
}