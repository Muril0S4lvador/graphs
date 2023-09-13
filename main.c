#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "graphviz_print/graphviz_print.h"

int main(){

    Graph *graph = graph_read_file_CVRPLIB();

    graph_print(graph);

    img_print_vertex(graph, "imgs/vertex.dot");

    img_print_graph(graph, "imgs/graph.dot");

    graph_destroy(graph);

    return 0;
}