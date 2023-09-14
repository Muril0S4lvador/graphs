#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>

#include "../graph/graph.h"
#include "../data/data.h"

typedef struct{
    int src, dest;
    weight weight;
} Kruskal;

Graph *kruskal_algorithm(Graph *g);


#endif