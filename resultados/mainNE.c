#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"
#include <time.h>

void name(Graph *g){
    int numTrucks;
    char *name = graph_return_name(g);
    sscanf(name, "%*[^k]k%d" , &numTrucks);
    printf("%s;", name);
}

void distanceToOptimal(double cost, double optimal){
    double difference;
    difference = (double)((cost - optimal) / optimal);
    char number[20];
    sprintf(number, "%lf", difference);
    for(int i = 0; i < strlen(number); i++)
        if(number[i] == '.') number[i] = ',';
    printf("%s;", number);
}

int main( int argc, char* argv[] ){

   double initial, final, best;

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    graph_Clarke_Wright_parallel_route(g);
    graph_enables_routes(g);

    initial = graph_return_total_cost(g);
    best = graph_return_optimal_cost(g);

    graph_Variable_Neighborhood_Search(g);

    graph_2opt(g);

    final = graph_return_total_cost(g);

    name(g);
    printf("%0.lf;", initial);
    printf("%0.lf;", final);
    printf("%0.lf;", best);
    distanceToOptimal(initial, best);
    distanceToOptimal(final, best);
    printf("\n");

    graph_destroy(g);

    return 0;
}