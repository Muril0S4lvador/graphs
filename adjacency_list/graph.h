#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>

#define UNDIRECTED 1
#define DIRECTED 0

typedef void data_type;
typedef int height_type;

typedef struct Adjacency{
    int vertice;
    height_type height;
    struct Adjacency *next;
} Adjacency;

typedef struct{
    data_type *data;
    Adjacency *head;
} Vertices;

typedef struct{
    int num_vertices; 
    int num_edge;
    Vertices *vertices;
} Graph;

Graph *graph_construct(int v);

Adjacency *graph_adjacency_create(int vertice, height_type height);

void graph_add_edge(Graph *g, int vertex, Adjacency *adj, int direction);

void graph_print(Graph *g);

void graph_destroy(Graph *g);

#endif