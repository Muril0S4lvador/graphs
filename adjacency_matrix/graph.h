#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>

#define DIRECTED 0
#define UNDIRECTED 1

typedef int** matrix;
typedef int height;

typedef struct{
    int num_vertices;
    int num_edges;
    matrix adj;
} Graph;

Graph *graph_construct(int v);

void graph_add_edge(Graph *g, int v1, int v2, height peso, int direction);

void graph_print(Graph *g);

void graph_destroy(Graph *g);

#endif