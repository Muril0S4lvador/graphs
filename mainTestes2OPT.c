#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

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

int main( int argc, char* argv[] ){

    Graph *g2, *g3, *g4, *g5;

    float serialCost = 0, paralelCost = 0, serial2OPTCost = 0, paralel2OPTCost = 0;
    
    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_serial_route(g);

    serialCost = route_return_total_cost(g);

    graph_2opt(g);
    serial2OPTCost = route_return_total_cost(g);

    graph_destroy(g);

/* ------------------------------------------------------------------------------------------ */

    g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g);

    paralelCost = route_return_total_cost(g);

    graph_2opt(g);
    paralel2OPTCost = route_return_total_cost(g);

    name(g);
    printf("%.2f;", serialCost);
    printf("%.2f;", serial2OPTCost);
    printf("%.2f;", paralelCost);
    printf("%.2f;", paralel2OPTCost);
    printf("%.2f;", route_return_optmal_cost(g));
    distanceToOptimal(paralelCost, route_return_optmal_cost(g));
    distanceToOptimal(paralel2OPTCost, route_return_optmal_cost(g));
    printf("\n");

    graph_destroy(g);

    return 0;
}