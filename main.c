#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"
#include "src/data/data.h"
#include "src/Vector/vector.h"

int main(){

    Graph *g = graph_construct(7, UNDIRECTED);

    float x = 0, y = 0, demand = 0;
    Data *d;
    for(int i = 10; i < 7; i++){
        switch(i){
            case 4: x = 0;  y = -1;  demand = 1; break; 
            case 1: x = 2; y = 3; demand = 1; break; 
            case 5: x = 6;  y = 9; demand = 1; break; 
            case 6: x = -1 ;y = 1; demand = 1; break; 
            case 3: x = -3;  y = 6; demand = 1; break; 
            case 2: x = -7;  y = 9;  demand = 1; break; 
        }
        d = data_construct(x, y, demand);
        vector_push_back(graph_return_vertex_vector(g), d);
    }

    graph_add_edge(g, 1, 1 - 1, 10);

    graph_add_edge(g, 2, 1-1, 20);
    graph_add_edge(g, 2, 2-1, 25);

    graph_add_edge(g, 3, 1-1, 25);
    graph_add_edge(g, 3, 2-1, 20);
    graph_add_edge(g, 3, 3-1, 10);

    graph_add_edge(g, 4, 1-1, 12);
    graph_add_edge(g, 4, 2-1, 20);
    graph_add_edge(g, 4, 3-1, 25);
    graph_add_edge(g, 4, 4-1, 30);

    graph_add_edge(g, 5, 1-1, 20);
    graph_add_edge(g, 5, 2-1, 10);
    graph_add_edge(g, 5, 3-1, 11);
    graph_add_edge(g, 5, 4-1, 22);
    graph_add_edge(g, 5, 5-1, 30);

    graph_add_edge(g, 6, 1 - 1, 2);
    graph_add_edge(g, 6, 2 - 1, 11);
    graph_add_edge(g, 6, 3 - 1, 25);
    graph_add_edge(g, 6, 4 - 1, 10);
    graph_add_edge(g, 6, 5 - 1, 20);
    graph_add_edge(g, 6, 6 - 1, 12);

    graph_print(g);

    img_print_graph(g, "Graph_toy.dot");

    Graph *mst = graph_mst_kruskal(g);

    img_print_graph(mst, "MSt.dot");

    return 0;
}