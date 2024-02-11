#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"
#include <time.h>

void name(Graph *g){
    int numTrucks;
    char *name = graph_return_name(g);
    sscanf(name, "%*[^k]k%d" , &numTrucks);
    printf("\n%s(%.lf):\n", name, graph_return_optimal_cost(g));
}

char checkRestriction(Graph *g){
    // char possible[20] = "POSSIBLE;";

    int size = graph_return_trucks(g),
        capacity = graph_return_capacity(g);

    for(int i = 0; i < size; i++){
        if( route_return_demand(g, i) > capacity ){
            // printf("%d ", route_return_demand(g, i));
            // strcpy(possible, "IMPOSSIBLE;");
            // break;
            return 0;
        }
    }
    return 1;
    // printf("%s", possible);
}

int main( int argc, char* argv[] ){

    int N = 10;
    double costs[N];

    for(int i = 0; i < N; i++){
        Graph *g = graph_read_file_CVRPLIB(argv[1]);
        if(!i) name(g);
        graph_Clarke_Wright_parallel_route(g);
        graph_enables_routes(g);
        graph_2opt(g);
        graph_Variable_Neighborhood_Search(g);

        costs[i] = graph_return_total_cost(g);
        
        graph_destroy(g);
    }

    printf("\n-----\nCosts per iteration:\n");
    for(int i = 0; i < N; i++) printf("%.2lf\n", costs[i]);
    printf("-----\n\n");

    return 0;
}