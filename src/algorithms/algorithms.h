#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>

#include "../graph/graph.h"

typedef struct{
    int src, dest;
    weight weight;
} Edges;

Graph *kruskal_algorithm(Edges *k, int num_vertex, int num_edges, Graph *g);

int kruskal_compare(const void* a, const void* b);

void dfs_algorithm(void *adj, int *route, int *visited, int size);

Graph *clarke_wright_algorithm(Graph *g, Edges *e, int sizeE);



#endif