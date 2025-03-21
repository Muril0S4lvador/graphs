#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "../adjacency_matrix/matrix.h"
#include "../route/route.h"

typedef unsigned char bool;

#define NEIGHBORHOOD_STRUCTURES 4
#define SHAKE_STRUCTURES 3

#define NUM_IT 1000

typedef struct Graph Graph;

struct Route;
typedef struct Route Route;

// Constrói um grafo
Graph *graph_construct(int v, bool direction);

// Retorna número de vértices de um grafo
int graph_return_num_vertex(Graph *g);

// Retorna número de arestas de um grafo
int graph_return_num_edges(Graph *g);

// Retorna a capacidade limite nas rotas de um grafo
int graph_return_capacity(Graph *g);

// Retorna a quantidade de caminhões/rotas de um grafo
int graph_return_trucks(Graph *g);

// Retorna a direção de um grafo
bool graph_return_direction(Graph *g);

// Retorna se existe uma rota para um grafo
bool graph_has_route(Graph *g);

// Retorna vetor de vértices de um grafo
void *graph_return_vertex_vector(Graph *g);

// Retorna as adjacências de um grafo
void *graph_return_adjacencies(Graph *g);

// Retorna a i-ésima rota de um grafo
Route *graph_return_route(Graph *g);

// Retorna o custo ótimo da instância lida
int graph_return_optimal_cost(Graph *g);

// Retorna o nome da instância lida
char *graph_return_name(Graph *g);

// Retorna a demanda total da instância lida
int graph_return_total_demand(Graph *g);

// Adiciona uma aresta (v1, v2) com peso em um grafo
void graph_add_edge(Graph *g, int v1, int v2, weight peso);

// Remove a aresta (v1, v2) em um grafo
void graph_remove_edge(Graph *g, int v1, int v2);

// Retorna se a aresta (v1, v2) existe no grafo
bool graph_edge_exists(Graph *g, int v1, int v2);

// Retorna um grafo de acordo com uma instância da CVRPLIB
Graph *graph_read_file_CVRPLIB(char *fileName);

// Retorna um grafo de acordo com um arquivo txt
Graph *graph_read_file();

// Imprime o grafo no terminal (Matriz de adjacências)
void graph_print(Graph *g);

// Retorna um grafo de MST pelo algoritmo de Kruskal de acordo com um grafo
Graph *graph_mst_kruskal(Graph *g);

// Cria uma rota de acordo com o algoritmo de Savings Parallel
void graph_Clarke_Wright_parallel_route(Graph *g);

// Cria uma rota de acordo com o algoritmo de Savings Serial
void graph_Clarke_Wright_serial_route(Graph *g);

// Atribui a rota ao grafo
void graph_set_route(Graph *g, int idx, void *route, int size, int demand);

// Retorna um vetor int com as demandas dos vértices do grafo
int *graph_return_demands(Graph *g);

// Imprime as rotas do grafo no terminal
void graph_print_routes(Graph *g);

// Verifica a viabilidade das rotas, se não desrespeitam algum limite
int graph_check_routes(char *filename, Graph *g);

// Melhora as rotas do grafo de acordo com o algoritmo 2OPT intra-rotas
void graph_2opt(Graph *g);

// Melhora as rotas de um grafo de acordo com o algoritmo VNS
void graph_Variable_Neighborhood_Search(Graph *g);

// Garante que todas as rotas de um grafo sejam viáveis
void graph_enables_routes(Graph *g);

// Desaloca as rotas de um grafo da memória
void graph_route_destroy(Graph *g);

// Desaloca um grafo da memória
void graph_destroy(Graph *g);

// Realiza o algoritmo de cross entre as rotas de um grafo
void graph_cross_exchange(Graph *g);

#endif