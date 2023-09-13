#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "adjacency_list/list.h"
#include "adjacency_matrix/matrix.h"
#include "Vector/vector.h"

#define MATRIX 0
#define LIST 1

typedef struct Graph Graph;

Graph *graph_construct(int v, bool direction);

int graph_return_num_vertex(Graph *g);

bool graph_return_direction(Graph *g);

Vector *graph_return_vertex_vector(Graph *g);

void *graph_return_adjacencies(Graph *g);

void graph_add_edge(Graph *g, int v1, int v2, weight peso);

void graph_remove_edge(Graph *g, int v1, int v2);

bool graph_edge_exists(Graph *g, int v1, int v2);

Graph *graph_read_file_CVRPLIB();

Graph *graph_read_file();

Graph *graph_kruskal(Graph *g);

void graph_print(Graph *g);

void graph_destroy(Graph *g);

#endif