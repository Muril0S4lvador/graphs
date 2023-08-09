#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){

    Graph *graph = graph_construct(5);

    graph_add_edge(graph, 0, adjacency_create(1, 2), DIRECTED);
    graph_add_edge(graph, 1, adjacency_create(2, 4), DIRECTED);
    graph_add_edge(graph, 2, adjacency_create(0, 12), DIRECTED);
    graph_add_edge(graph, 2, adjacency_create(4, 40), DIRECTED);
    graph_add_edge(graph, 3, adjacency_create(1, 3), DIRECTED);
    graph_add_edge(graph, 4, adjacency_create(3, 8), DIRECTED);

    graph_print(graph);

    graph_destroy(graph);

    return 0;
}
