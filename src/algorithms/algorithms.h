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

// Retorna um grafo contendo uma MST de acordo com o algoritmo de Kruskal
Graph *kruskal_algorithm(Edges *k, int num_vertex, int num_edges, Graph *g);

// Função para ordenar crescente um vetor de Edges (qsort)
int edges_compare_growing(const void* a, const void* b);

// Função para ordenar decrescente um vetor de Edges (qsort)
int edges_compare_descending(const void* a, const void* b);

// Cria uma rota de acordo com o algoritmo de Savings, versão serial
void clarke_wright_serial_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges);

// Cria uma rota de acordo com o algoritmo de Savings, versão paralela
void clarke_wright_parallel_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges);

// Melhora uma rota já criada de acordo com o algoritmo de 2OPT
void opt2_algorithm(int *route, int sizeRoute, void *graph_adj, double *cost);

// Melhora uma rota já criada de acordo com o algoritmo de busca de vizinho variável
void variable_Neighborhood_Search(Graph *g, int **routes, int *sizeRoutes, int *demands, int *demandRoutes);

// Melhora uma rota já criada de acordo com o algoritmo de descida em vizinhanças
int **variable_Neighborhood_Descent(int **routes, int *sizeRoutes, int *idx_InRoutes, int *demandRoutes, double *costRoutes, int *demands, Graph *g);

// Busca viabilizar uma rota com o método de troca de operadores
void enables_route_swap(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int capacity, double *cost, Graph *g);

// Busca viabilizar uma rota com o método de realocação de operadores
void enables_route_reallocate(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int capacity, double *cost, Graph *g);



/* =============================================== APAGAR DEPOIS ================================================================== */

char opt2_inter_routes(int **routes, int size, int *sizeR, int *demandR, double *cost, int *demands, Graph *g);

// Printa todas as rotas
void printsd(int **routes, int size, int *sizeR, int *demandR, double *cost, int *demands);
// void printsd(int **routes, int size, int *sizeR, int *demandR);

#endif