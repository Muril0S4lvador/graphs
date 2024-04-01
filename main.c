#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include "src/algorithms/algorithms.h"
#include "src/adjacency_matrix/matrix.h"

void checkRestriction(Graph *g){
    char possible[20] = "POSSIBLE;";

    int size = graph_return_trucks(g),
        capacity = graph_return_capacity(g);

    for(int i = 0; i < size; i++){
        if( route_return_demand(g, i) > capacity ){
            // printf("%d ", route_return_demand(g, i));
            strcpy(possible, "IMPOSSIBLE;");
            break;
        }
    }

    printf("%s", possible);
}

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    // graph_Clarke_Wright_parallel_route(g);
    
    // route_print(g);


    // graph_enables_routes(g);


    // graph_destroy(g);

    // g = graph_read_file_CVRPLIB(argv[1]);

    graph_Clarke_Wright_serial_route(g);
    graph_2opt(g);

    img_print_route(g, "imgs/rota");

    printf("%0.lf;\n", graph_return_total_cost(g));

    // graph_enables_routes(g);

    // graph_2opt(g);

    graph_destroy(g);

    return 0;
}

/*
valgrind ./main com entradas/a/A-n46-k7.vrp
*/
