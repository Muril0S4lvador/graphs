#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){

    Graph *graph = graph_read_file();
    /*
    exit(-2);

    Graph *graph = graph_construct(5);

    graph_add_edge(graph, 0, 1, WEIGHT_DEFAULT, DIRECTED);
    graph_add_edge(graph, 1, 2, WEIGHT_DEFAULT, DIRECTED);
    graph_add_edge(graph, 2, 0, WEIGHT_DEFAULT, DIRECTED);
    graph_add_edge(graph, 2, 4, WEIGHT_DEFAULT, DIRECTED);
    graph_add_edge(graph, 3, 1, WEIGHT_DEFAULT, DIRECTED);
    graph_add_edge(graph, 4, 3, WEIGHT_DEFAULT, DIRECTED);
    */

    graph_print(graph);
    

    graph_destroy(graph);

    return 0;
}