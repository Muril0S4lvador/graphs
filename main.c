#include <stdio.h>
#include <stdlib.h>
#include "src/graph/graph.h"
#include "src/info/info.h"
#include "src/graphviz_print/graphviz_print.h"

#include <time.h>

void distanceToOptimal(double cost, double optimal){
    double difference;
    difference = (double)((cost - optimal) / optimal) * 100;
    printf("%.0lf %.0lf (%.2lf%%)\n", cost, optimal, difference);
}

int main( int argc, char* argv[] ){

    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    int times = 10;
    int seed  = 0;
    FILE *f = fopen("entradas/seeds.bin", "rb");

    Info **arr = info_array_construct(times);

    for(int i = 0; i < times; i++){

        info_define(arr, i);
        info_construct(g);

        fread(&seed, sizeof(int), 1, f);
        srand(seed);
        info_set_seed(seed);

        // printf("%d\n", seed);
        
        graph_Clarke_Wright_parallel_route(g);
        info_set_cost_constructive(route_return_total_cost(graph_return_route(g), graph_return_trucks(g)));

        graph_enables_routes(g);
        info_set_cost_enables(route_return_total_cost(graph_return_route(g), graph_return_trucks(g)));

        graph_Variable_Neighborhood_Search(g);
        info_set_cost_vns(route_return_total_cost(graph_return_route(g), graph_return_trucks(g)));
        
        info_set_routes(graph_return_route(g));

        graph_route_destroy(g);
    }
    fclose(f);

    info_print_table_result(arr, times);
    info_print_table_infos(arr, times);

    info_print_arr_file(arr, times);
    info_print_solution_file(arr, times);
    info_print_results_file(arr, times);


    graph_destroy(g);
    info_arr_destroy(arr, times);

    return 0;
}