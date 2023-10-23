#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/graph/graph.h"
#include "src/graphviz_print/graphviz_print.h"

// Name;Serial Cost; time; Paralel cost; time; Optimal time
int main( int argc, char* argv[] ){

    clock_t start_time, end_time;
    double elapsed_time_ms;

    start_time = clock(); // Marca o início do tempo
    Graph *g = graph_read_file_CVRPLIB(argv[1]);

    graph_Clarke_Wright_serial_route(g);
    end_time = clock(); // Marca o fim do tempo

    printf("%s;", graph_return_name(g));
    printf("%.2f;", route_return_total_cost(g));
    printf("%.2f;", (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC);

    graph_destroy(g);

    start_time = clock(); // Marca o início do tempo
    g = graph_read_file_CVRPLIB(argv[1]);

    graph_Clarke_Wright_paralel_route(g);
    end_time = clock(); // Marca o fim do tempo

    printf("%.2f;", route_return_total_cost(g));
    
    printf("%.2f;", (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC);

    printf("%.2f\n", route_return_optmal_cost(g));

    img_print_route(g, "imgs/route.dot");

    graph_destroy(g);

    return 0;
}