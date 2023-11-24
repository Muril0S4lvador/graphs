#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../graph/graph.h"

typedef struct{
    int src, dest;
    weight weight;
} Edges;

Graph *kruskal_algorithm(Edges *k, int num_vertex, int num_edges, Graph *g);

int edges_compare_growing(const void* a, const void* b);

int edges_compare_descending(const void* a, const void* b);

void clarke_wright_serial_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges);

void clarke_wright_parallel_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges);

void opt2_algorithm(int *route, int sizeRoute, void *graph_adj);

int** first_improvement(Graph *g, int **routes, int *sizeRoute, float *demands, int k);

void variable_Neighborhood_Search(Graph *g, int **routes, int *sizeRoutes, float *demands, int *demandRoutes);

int **neighborhood_Change_Cyclic(int **currentSolution, int **testSolution, int *k, float testCost, float *currentCost);

#endif