#ifndef _GRAPHVIZ_PRINT_H_
#define _GRAPHVIZ_PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include "../graph/graph.h"

#define TYPE "png"

void img_print_vertex(Graph *g, char *file_name);

void img_print_graph(Graph *g, char *file_name);

void img_print_graph_per_edge(Graph *g1, Graph *g2, int it, char *file_name);

#endif