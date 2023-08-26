#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>

#define UNDIRECTED 1
#define DIRECTED 0
#define HEIGHT_DEFAULT 1

typedef void data_type;
typedef int height;

typedef struct Adjacency Adjacency;
typedef struct List* List;

List list_construct(int v);

Adjacency *adjacency_create(int vertice, height height);

void list_add_edge(void *vl, int v1, int v2, height peso, int direction);

void list_print(void *vl, int size);

void list_destroy(void *vl, int size);

#endif