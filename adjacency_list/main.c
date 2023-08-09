#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){

    Graph *graph = graph_construct(5);

    edge_create(graph, 0, adjacency_create(1, 2), UNDIRECTED);
    edge_create(graph, 1, adjacency_create(2, 4), UNDIRECTED);
    edge_create(graph, 2, adjacency_create(0, 12), UNDIRECTED);
    edge_create(graph, 2, adjacency_create(4, 40), UNDIRECTED);
    edge_create(graph, 3, adjacency_create(1, 3), UNDIRECTED);
    edge_create(graph, 4, adjacency_create(3, 8), UNDIRECTED);

    graph_print(graph);

    graph_destroy(graph);

    return 0;
}