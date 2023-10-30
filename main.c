#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g);

    graph_destroy(g);

    return 0;
}
/*
A-n39-k6
A-n45-k6
A-n61-k9
A-n63-k9

B-n45-k6
B-n51-k7
B-n57-k7
B-n64-k9

E-n101-k14
E-n13-k4
E-n23-k3
E-n51-k5
E-n76-k10
E-n76-k14
*/