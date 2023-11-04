#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef unsigned char bool;
typedef float weight;

#define UNDIRECTED 1
#define DIRECTED 0

typedef struct Graph Graph;
typedef struct Route Route;

Graph *graph_construct(int v, bool direction);

int graph_return_num_vertex(Graph *g);

int graph_return_num_edges(Graph *g);

int graph_return_capacity(Graph *g);

int graph_return_trucks(Graph *g);

void graph_set_trucks(Graph *g, int numTrucks);

bool graph_return_direction(Graph *g);

bool graph_has_route(Graph *g);

void *graph_return_vertex_vector(Graph *g);

void *graph_return_adjacencies(Graph *g);

void *route_return_route(Graph *g, int i);

int route_return_size(Graph *g, int i);

int route_return_demand(Graph *g, int i);

float route_return_cost(Graph *g, int i);

float route_return_total_cost(Graph *g);

float route_return_optimal_cost(Graph *g);

char *graph_return_name(Graph *g);

void graph_add_edge(Graph *g, int v1, int v2, weight peso);

void graph_remove_edge(Graph *g, int v1, int v2);

bool graph_edge_exists(Graph *g, int v1, int v2);

Graph *graph_read_file_CVRPLIB(char *fileName);

Graph *graph_read_file();

void graph_print(Graph *g);

Graph *graph_mst_kruskal(Graph *g);

void graph_Clarke_Wright_paralel_route(Graph *g);

void graph_Clarke_Wright_serial_route(Graph *g);

void graph_set_route(Graph *g, int idx, void *route, int size, float demand);

void graph_2opt(Graph *g);

void graph_destroy(Graph *g);

#endif