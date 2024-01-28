#include <stdio.h>
#include <stdlib.h>
#include "../src/graph/graph.h"
#include "../src/graphviz_print/graphviz_print.h"
#include <time.h>

void name(Graph *g){
    int numTrucks;
    char *name = graph_return_name(g);
    sscanf(name, "%*[^k]k%d" , &numTrucks);
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
    clock_t start, end;
    double time;
    
    Graph *g = graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_parallel_route(g);
    graph_2opt(g);

    befCost = graph_return_total_cost(g);

    name(g);
    printf("%.2f;", befCost);
    checkRestriction(g);

    start = clock();

    graph_enables_routes(g);

    end = clock();
    time = (double)(end-start);
    
    graph_2opt(g);
    AftCost = graph_return_total_cost(g);
    printf("%.2f;", AftCost);
    checkRestriction(g);

    printf("%.2f;", route_return_optimal_cost(g));
    
    distanceToOptimal(befCost, route_return_optimal_cost(g));
    distanceToOptimal(AftCost, route_return_optimal_cost(g));
    printf("%lf", time);
    printf("\n");

    graph_destroy(g);

    return 0;
}