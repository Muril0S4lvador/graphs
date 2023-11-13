#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

void checkTrucks(Graph *g){
    int numTrucks;
    char *name = graph_return_name(g);
    sscanf(name, "%*[^k]k%d" , &numTrucks);

    if( numTrucks < graph_return_trucks(g) ){
        route_print(g);
    }
}

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g);

    checkTrucks(g);
    graph_Variable_Neighborhood_Search(g);

    return 0;
}