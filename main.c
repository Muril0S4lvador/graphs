#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){

    Graph *graph = graph_read_file_CVRPLIB();

    graph_print(graph);

    graph_img_print_vertex(graph, "imgs/vertex.dot");

    graph_destroy(graph);

    return 0;
}