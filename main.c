#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main(){

    Graph *graph = graph_read_file_CVRPLIB();

    // graph_print(graph);

    // img_print_vertex(graph, "imgs/vertex.dot");

    // img_print_graph(graph, "imgs/graph.dot");

    Graph *g = graph_mst_kruskal(graph);

    // graph_print(g);

    img_print_graph(g, "imgs/mst.dot");

    graph_destroy(graph);
    graph_destroy(g);

    return 0;
}