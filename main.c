#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main(){

    Graph *graph = graph_read_file_CVRPLIB();

    Graph *g = graph_mst_kruskal(graph);

    graph_dfs(g);

    graph_print(g);

    img_print_graph(g, "imgs/route.dot");

    graph_destroy(graph);
    graph_destroy(g);

    return 0;
}