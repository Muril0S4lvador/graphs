#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

#include "src/algorithms/algorithms.h"
#include "src/adjacency_matrix/matrix.h"

#include <time.h>

#define size 5

int optimal = -1;
int cost[size];

void printcost(int custo){
    char control = 0;
    for(int i = 0; i < size - 1; i++) if(cost[i] == optimal) control = 1;

    if( custo == optimal && control ){
        printf("\\textbf{%d} ", custo);
        
    } else if(custo == -1){
        printf("--");

    } else {
        printf("%d ", custo);

    }
}

void printname(char *name){
    char control = 0;
    for(int i = 0; i < size - 1; i++) if(cost[i] == optimal) control = 1;

    if( control ){
        printf("\\textbf{%s} ", name);
    } else {
        printf("%s ", name);

    }
}

// int main( int argc, char* argv[] ){

//     Graph *g = graph_read_file_CVRPLIB(argv[1]);

//     optimal = graph_return_optimal_cost(g);

//     graph_Clarke_Wright_parallel_route(g);
//     cost[0] = graph_return_total_cost(g);

//     graph_enables_routes(g);
//     cost[1] = graph_return_total_cost(g);

//     graph_2opt(g);
//     cost[2] = graph_return_total_cost(g);

// if( graph_return_num_vertex(g) == 135 ){
//     cost[3] = -1;
// } else{

//     graph_Variable_Neighborhood_Search(g);
//     cost[3] = graph_return_total_cost(g);
// }

//     cost[4] = optimal;

//     printname(graph_return_name(g));
//     for(int i = 0; i < size; i++){
//         printf("& ");
//         printcost(cost[i]);
//     }

//     printf(" \\\\\n");


//     graph_destroy(g);

//     return 0;
// }

void reset(Graph *g, char *argv[]){
    graph_destroy(g);
    graph_read_file_CVRPLIB(argv[1]);
    graph_Clarke_Wright_parallel_route(g);
}

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    optimal = graph_return_optimal_cost(g);

    graph_Clarke_Wright_parallel_route(g);
    cost[0] = graph_return_total_cost(g);

            reset(g, argv);

    graph_enables_routes(g);
    cost[1] = graph_return_total_cost(g);

            reset(g, argv);

    graph_2opt(g);
    cost[2] = graph_return_total_cost(g);

            reset(g, argv);

if( graph_return_num_vertex(g) == 135 ){
    cost[3] = -1;
} else{

    graph_Variable_Neighborhood_Search(g);
    cost[3] = graph_return_total_cost(g);
}

    cost[4] = optimal;

    printname(graph_return_name(g));
    for(int i = 0; i < size; i++){
        printf("& ");
        printcost(cost[i]);
    }

    printf(" \\\\\n");


    graph_destroy(g);

    return 0;
}