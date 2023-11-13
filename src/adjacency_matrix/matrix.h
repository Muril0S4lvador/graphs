#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>

#include "../data/data.h"

#define UNDIRECTED 1
#define DIRECTED 0
#define WEIGHT_DEFAULT 1

typedef float weight;
typedef float** Matrix;

Matrix matrix_construct(int v);

char matrix_add_edge(void *vm, int v1, int v2, weight peso);

void matrix_remove_edge(void *vm, int v1, int v2);

char matrix_edge_exists(void *vm, int v1, int v2);

float matrix_return_edge_weight(void *vm, int v1, int v2, int direction);

void matrix_print(void *vm, int size);

void matrix_file_write(void *vm, int size, FILE *arq, char *edge);

void matrix_return_edges(void *vm, int sizeVertex, void *vk, int direction);

void matrix_return_edges_savings(void *vm, int sizeVertex, void *vk, void *vn);

float matrix_return_route_cost(void *vm, int *route, int size_route);

void matrix_destroy(void *vm, int size);

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