#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include "src/algorithms/algorithms.h"
#include "src/adjacency_matrix/matrix.h"

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    graph_Clarke_Wright_parallel_route(g);

    graph_Variable_Neighborhood_Search(g);

    int size = graph_return_trucks(g);
    float sum = 0;
    for(int i = 0; i < size; i++){
        // int *ha = malloc(sizeof(int) * route_return_size(g, i));
        int *ha;
        ha = route_return_route(g, i);
        float cost = 0;
        int tam = route_return_size(g, i);

        cost = matrix_return_route_cost(graph_return_adjacencies(g), ha, tam);
        // printf("%d : %.3f\n", i, cost);
        sum += cost;

    }
    // printf("%.3f\n", sum);
    
    graph_destroy(g);

    return 0;
}

/*
valgrind ./main com entradas/a/A-n46-k7.vrp
*/