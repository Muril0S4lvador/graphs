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

    FILE *arq = fopen("medias instancias.csv", "a");

    printf("%c;%d;%d;%d;%d;\n", graph_return_name(g)[0], graph_return_num_vertex(g), graph_return_trucks(g), graph_return_capacity(g), graph_return_optimal_cost(g));

    fclose(arq);
    graph_destroy(g);

    return 0;
}
