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

void graph_add_edge(Graph *g, int v1, int v2, weight peso);

void graph_remove_edge(Graph *g, int v1, int v2);

Graph *graph_read_file_CVRPLIB();

Graph *graph_read_file();

Graph *graph_kruskal(Graph *g);

void graph_print(Graph *g);

void graph_img_print_vertex(Graph *g, char *file_name);

void graph_destroy(Graph *g);

#endif