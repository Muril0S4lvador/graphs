#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>

#include "../data/data.h"

#define UNDIRECTED 1
#define DIRECTED 0
#define WEIGHT_DEFAULT 1

typedef float weight;
// typedef void data_type;

typedef struct Adjacency Adjacency;
typedef struct List* List;

List list_construct(int v);

Adjacency *adjacency_create(int vertice, weight weight);

void list_add_edge(void *vl, int v1, int v2, weight peso, int direction);

void list_print(void *vl, int size);

void list_destroy(void *vl, int size);

#endif