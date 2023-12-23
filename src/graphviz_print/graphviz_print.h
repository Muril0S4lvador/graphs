#ifndef _GRAPHVIZ_PRINT_H_
#define _GRAPHVIZ_PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include "../graph/graph.h"

#define TYPE "png"

// Imprime no arquivo <file_name>.png os vértices do grafo g
void img_print_vertex(Graph *g, char *file_name);

// Imprime no arquivo <file_name>.png o grafo g
void img_print_graph(Graph *g, char *file_name);

// Imprime no arquivo <file_name>.png a rota do grafo g
void img_print_route(Graph *g, char *file_name);

// Imprime no arquivo <file_name>.png cada aresta nova do grafo g
void img_print_graph_per_edge(Graph *g1, Graph *g2, int it, char *file_name);

#endif