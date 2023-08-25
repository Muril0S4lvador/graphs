#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIRECTED 0
#define UNDIRECTED 1
#define HEIGHT_DEFAULT 1

typedef int** matrix;
typedef int height;

typedef struct{
    int num_vertices;
    int num_edges;
    matrix adj;
} Graph;

Graph *graph_construct(int v);

void graph_add_edge(Graph *g, int v1, int v2, height peso, int direction);

Graph *graph_read_file(Graph *g);

void graph_print(Graph *g);

void graph_destroy(Graph *g);

#endif

/*
NAME: berlin52
COMMENT: 52 locations in Berlin (Groetschel)
TYPE: TSP
DIMENSION: 52
EDGE_WEIGHT_TYPE: EUC_2D
NODE_COORD_SECTION
1 565.0 575.0
*/