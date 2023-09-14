#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>

#include "../graph/graph.h"

typedef struct{
    int src, dest;
    weight weight;
} Kruskal;

Graph *kruskal_algorithm(Kruskal *k, int num_vertex, int num_edges);

int kruskal_compare(const void* a, const void* b);


#endif