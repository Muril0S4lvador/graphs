#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

// Name;Serial Cost; time; Paralel cost; time; Optimal;
int main( int argc, char* argv[] ){

    Graph *g2, *g3, *g4, *g5;
    clock_t start_time, end_time;

    float serialCost = 0, paralelCost = 0;
    double serialTime = 0, paralelTime = 0;
    
    start_time = clock();
    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_serial_route(g);
    end_time = clock();

    serialTime += (double)(end_time - start_time);

    start_time = clock();
    g2 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_serial_route(g2);
    end_time = clock();

    serialTime += (double)(end_time - start_time);

    start_time = clock();
    g3 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_serial_route(g3);
    end_time = clock();

    serialTime += (double)(end_time - start_time);

    start_time = clock();
    g4 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_serial_route(g4);
    end_time = clock();
    
    serialTime += (double)(end_time - start_time);

    start_time = clock();
    g5 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_serial_route(g5);
    end_time = clock();

    serialTime += (double)(end_time - start_time);

    serialCost = route_return_total_cost(g);
    serialTime /= 5;

    graph_destroy(g);
    graph_destroy(g2);
    graph_destroy(g3);
    graph_destroy(g4);
    graph_destroy(g5);

/* ------------------------------------------------------------------------------------------ */

    start_time = clock();
    g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g);
    end_time = clock();

    paralelTime += (double)(end_time - start_time);

    start_time = clock();
    g2 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g2);
    end_time = clock();

    paralelTime += (double)(end_time - start_time);

    start_time = clock();
    g3 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g3);
    end_time = clock();

    paralelTime += (double)(end_time - start_time);

    start_time = clock();
    g4 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g4);
    end_time = clock();
    
    paralelTime += (double)(end_time - start_time);

    start_time = clock();
    g5 = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_paralel_route(g5);
    end_time = clock();

    paralelTime += (double)(end_time - start_time);

    paralelCost = route_return_total_cost(g);
    paralelTime /= 5;

    name(g);
    printf("%.2f;", serialCost);
    printf("%.2lf;", serialTime);
    printf("%.2f;", paralelCost);
    printf("%.2lf;", paralelTime);
    printf("%.2f;", route_return_optmal_cost(g));
    distanceToOptimal(paralelCost, route_return_optmal_cost(g));
    printf("\n");

    graph_destroy(g);
    graph_destroy(g2);
    graph_destroy(g3);
    graph_destroy(g4);
    graph_destroy(g5);

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