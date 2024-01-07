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

    int *demands = graph_return_demands(g);
    int soma = 0;
    for(int murilo = 0; murilo < graph_return_num_vertex(g); murilo++){
        soma += demands[murilo];                
    }
    printf("Demanda Total: %d | %d\n", soma, graph_return_capacity(g));

    int *v = calloc(sizeof(int), graph_return_num_vertex(g));

    int size = graph_return_trucks(g);
    float sum = 0;
    for(int i = 0; i < size; i++){
        // int *ha = malloc(sizeof(int) * route_return_size(g, i));
        // break;
        int *ha;
        ha = route_return_route(g, i);
        float cost = 0;
        int tam = route_return_size(g, i);

        cost = matrix_return_route_cost(graph_return_adjacencies(g), ha, tam);
        sum += cost;

        int d = 0;
        for(int j = 0; j < tam; j++){
            d += demands[ha[j]];
            v[ha[j]]++;
        }

        printf("%d : %.3f || %d\n", i, cost, d);

    }
    printf("%.3f\n", sum);

    for(int i = 0; i < graph_return_num_vertex(g); i++){
        // printf("%02d : %01d\n", i, v[i]);
        // printf("%d : %f\n", i, matrix_return_edge_weight(graph_return_adjacencies(g), 0, i, UNDIRECTED));
    }
    
    graph_destroy(g);

    return 0;
}

/*
valgrind ./main com entradas/a/A-n46-k7.vrp
*/