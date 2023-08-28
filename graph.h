#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "adjacency_list/list.h"
#include "adjacency_matrix/matrix.h"
#include "Vector/vector.h"

#define MATRIX 1
#define LIST 1

typedef struct Graph Graph;

Graph *graph_construct(int v);

void graph_add_edge(Graph *g, int v1, int v2, weight peso, int direction);

Graph *graph_read_file_CVRPLIB();

Graph *graph_read_file();

void graph_print(Graph *g);

void graph_destroy(Graph *g);

#endif