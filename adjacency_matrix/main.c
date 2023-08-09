#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){

    Graph *graph = graph_construct(500);

    graph_add_edge(graph, 0, 1, 2, DIRECTED);
    graph_add_edge(graph, 1, 2, 4, DIRECTED);
    graph_add_edge(graph, 2, 0, 12, DIRECTED);
    graph_add_edge(graph, 2, 4, 40, DIRECTED);
    graph_add_edge(graph, 3, 1, 3, DIRECTED);
    graph_add_edge(graph, 4, 3, 8, DIRECTED);

    graph_print(graph);

    graph_destroy(graph);

    return 0;
}