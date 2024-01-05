#include <stdio.h>
#include <stdlib.h>
#include "../../src/graph/graph.h"
#include "../../src/graphviz_print/graphviz_print.h"

void name(Graph *g){
    int numTrucks;
    char *name = graph_return_name(g);
    sscanf(name, "%*[^k]k%d" , &numTrucks);

    if( numTrucks < graph_return_trucks(g) ){
        printf("*");
    }
    printf("%s;", name);
}

void distanceToOptimal(float cost, float optimal){
    double difference;
    difference = (double)((cost - optimal) / optimal);
    printf("%.6lf;", difference);
}

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

    float befCost = 0, AftCost = 0;
    
    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_parallel_route(g);

    befCost = route_return_total_cost(g);

    name(g);
    printf("%.2f;", befCost);
    checkRestriction(g);

    graph_Variable_Neighborhood_Search(g);

    AftCost = route_return_total_cost(g);

    printf("%.2f;", AftCost);
    checkRestriction(g);

    printf("%.2f;", route_return_optimal_cost(g));
    
    distanceToOptimal(befCost, route_return_optimal_cost(g));
    distanceToOptimal(AftCost, route_return_optimal_cost(g));
    printf("\n");

    graph_destroy(g);

    return 0;
}