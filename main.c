#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include "src/algorithms/algorithms.h"
#include "src/adjacency_matrix/matrix.h"

void checkRestriction(Graph *g){
    char possible[20] = "POSSIBLE;";

    int size = graph_return_trucks(g),
        capacity = graph_return_capacity(g);

    for(int i = 0; i < size; i++){
        if( route_return_demand(g, i) > capacity ){
            // printf("%d ", route_return_demand(g, i));
            strcpy(possible, "IMPOSSIBLE;");
            break;
        }
    }

    printf("%s", possible);
}

int main( int argc, char* argv[] ){

    double constCost, optCost, vnsCost, bestCost;

    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_parallel_route(g);
    
    printf("\nCapacity: %d\n",graph_return_capacity(g));

    constCost = graph_return_total_cost(g);
    bestCost = graph_return_optimal_cost(g);

    graph_enables_routes(g);
    graph_2opt(g);

    optCost = graph_return_total_cost(g);

    graph_Variable_Neighborhood_Search(g);

    vnsCost = graph_return_total_cost(g);

    graph_destroy(g);

    printf("\n---------------------------\nConclusion:\n\nConstructive: %lf\n2Opt:         %lf\nVNS:          %lf\nBest:         %.0lf\n", constCost, optCost, vnsCost, bestCost);

    return 0;
}

/*
valgrind ./main com entradas/a/A-n46-k7.vrp
*/