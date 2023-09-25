#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main(){

    Graph *g = graph_construct(7, UNDIRECTED);

    graph_add_edge(g, 0, 1, 10);

    graph_add_edge(g, 0, 2, 20);
    graph_add_edge(g, 1, 2, 25);

    graph_add_edge(g, 0, 3, 25);
    graph_add_edge(g, 1, 3, 20);
    graph_add_edge(g, 2, 3, 10);

    graph_add_edge(g, 0, 4, 12);
    graph_add_edge(g, 1, 4, 20);
    graph_add_edge(g, 2, 4, 25);
    graph_add_edge(g, 3, 4, 30);

    graph_add_edge(g, 0, 5, 20);
    graph_add_edge(g, 1, 5, 10);
    graph_add_edge(g, 2, 5, 11);
    graph_add_edge(g, 3, 5, 22);
    graph_add_edge(g, 4, 5, 30);

    graph_add_edge(g, 0, 6, 2);
    graph_add_edge(g, 1, 6, 11);
    graph_add_edge(g, 2, 6, 25);
    graph_add_edge(g, 3, 6, 10);
    graph_add_edge(g, 4, 6, 20);
    graph_add_edge(g, 5, 6, 12);

    img_print_graph(g, "graph.dot");

    Graph *r1 = graph_construct(7, DIRECTED);

    graph_add_edge(r1, 0, 1, 10);
    graph_add_edge(r1, 1, 0, 10);

    graph_add_edge(r1, 0, 2, 20);
    graph_add_edge(r1, 2, 0, 20);

    graph_add_edge(r1, 0, 3, 25);
    graph_add_edge(r1, 3, 0, 25);

    graph_add_edge(r1, 0, 4, 12);
    graph_add_edge(r1, 4, 0, 12);

    graph_add_edge(r1, 0, 5, 20);
    graph_add_edge(r1, 5, 0, 20);

    graph_add_edge(r1, 0, 6, 2);
    graph_add_edge(r1, 6, 0, 2);

    img_print_graph(r1, "r1.dot");

    Graph *r2 = graph_construct(7, DIRECTED);

    graph_add_edge(r2, 0, 1, 10);
    graph_add_edge(r2, 1, 0, 10);
    
    graph_add_edge(r2, 0, 2, 20);
    graph_add_edge(r2, 2, 3, 35);
    graph_add_edge(r2, 3, 0, 25);

    graph_add_edge(r2, 0, 4, 12);
    graph_add_edge(r2, 4, 0, 12);

    graph_add_edge(r2, 0, 5, 20);
    graph_add_edge(r2, 5, 0, 20);

    graph_add_edge(r2, 0, 6, 2);
    graph_add_edge(r2, 6, 0, 2);

    img_print_graph(r2, "r2.dot");

    Graph *r3 = graph_construct(7, DIRECTED);

    graph_add_edge(r3, 0, 5, 20);
    graph_add_edge(r3, 5, 2, 29);
    graph_add_edge(r3, 2, 3, 35);
    graph_add_edge(r3, 3, 0, 25);

    graph_add_edge(r3, 0, 4, 2);
    graph_add_edge(r3, 4, 1, 2);
    graph_add_edge(r3, 1, 6, 1);
    graph_add_edge(r3, 6, 0, 12);

    img_print_graph(r3, "r3.dot");

    free(g);
    free(r1);
    free(r2);
    free(r3);

    return 0;
}