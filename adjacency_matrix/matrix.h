#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>

#include "../data/data.h"

#define UNDIRECTED 1
#define DIRECTED 0
#define WEIGHT_DEFAULT 1

typedef float weight;
typedef int** Matrix;

Matrix matrix_construct(int v);

void matrix_add_edge(void *vm, int v1, int v2, weight peso, int direction);

void matrix_print(void *vm, int size);

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