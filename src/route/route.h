#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Route Route;
typedef unsigned char bool;
typedef int costType;

// Tipagem de impreção do custo da rota
#define COST_PRINT "%d"

// Constrói uma rota de acordo com o tamanho
Route* route_construct(int size);

// Retorna a rota de acordo com seu índice
int *route_return_route(Route *r, int index);

// Retorna o tamanho de uma rota de acordo com seu índice
int route_return_size(Route *r, int index);

// Retorna a demanda de uma rota de acordo com seu índice
int route_return_demand(Route *r, int index);

// Retorna o custo de uma rota de acordo com seu índice
costType route_return_cost(Route *r, int index);

// Retorna o custo total de uma matriz de rotas
costType route_return_total_cost(Route *r, int size);

// Atribui a ordem de uma rota de acordo com seu índice
void route_set_route(Route *r, int *route, int index, int size);

// Atribui o tamanho de uma rota de acordo com seu índice
void route_set_size(Route *r, int size, int index);

// Atribui a demanda de uma rota de acordo com seu índice
void route_set_demand(Route *r, int demand, int index);

// Atribui o custo de uma rota de acordo com seu índice
void route_set_cost(Route *r, costType cost, int index);

// Imprime uma rota de acordo com seu índice
void route_print(Route *r, int index);

// Imprime todas as rotas de uma matriz de rotas r em um arquivo arq
void route_print_file(Route *r, int size, FILE *arq);
    
// Libera a memória de todas as rotas de uma matriz de rotas r
void route_destroy(Route *r, int size);

#endif