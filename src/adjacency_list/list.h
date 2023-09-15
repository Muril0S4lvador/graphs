#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../data/data.h"

#define UNDIRECTED 1
#define DIRECTED 0
#define WEIGHT_DEFAULT 1

typedef float weight;
typedef struct Adjacency Adjacency;
typedef struct List* List;

List list_construct(int v);

Adjacency *adjacency_create(int vertice, weight weight);

char list_add_edge(void *vl, int v1, int v2, weight peso);

void list_remove_edge(void *vl, int v1, int v2);

char list_edge_exists(void *vl, int v1, int v2);

void list_print(void *vl, int size);

void list_file_write(void *vl, int size, FILE *arq, char *edge);

void list_return_kruskal(void *vl, int sizeVertex, int sizeEdges, void *vk);

void list_dfs_recursive(void *vl, int *route, int *size_route, int *visited);

void _list_dfs_recursive(Adjacency *adj, void *vl, int *route, int *size_route, int *visited);

void list_destroy(void *vl, int size);

#endif