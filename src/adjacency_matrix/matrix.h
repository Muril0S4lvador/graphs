#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include <stdlib.h>

#include "../data/data.h"

#define UNDIRECTED 1
#define DIRECTED 0
#define WEIGHT_DEFAULT 1

typedef double weight;
typedef double** Matrix;

// Constrói uma matriz VxV
Matrix matrix_construct(int v);

// Adiciona aresta (v1, v2) com peso, retorna bool se adicionou com sucesso
char matrix_add_edge(void *vm, int v1, int v2, weight peso);

// Remove aresta (v1, v2)
void matrix_remove_edge(void *vm, int v1, int v2);

// Retorna bool se aresta (v1, v2) existe
char matrix_edge_exists(void *vm, int v1, int v2);

// Retorna peso da aresta (v1, v2)
double matrix_return_edge_weight(void *vm, int v1, int v2, int direction);

// Imprime a matriz de adjacências no arquivo Matriz.txt
void matrix_print(void *vm, int size, FILE *arq);

// Escreve as arestas de cada vértice de acordo com a matriz no arquivo
void matrix_file_write(void *vm, int size, FILE *arq, char *edge);

// Preenche o vetor de Edges vk com as arestas da matriz
void matrix_return_edges(void *vm, int sizeVertex, void *vk, int direction);

// Retorna um vetor de Edges com as economias de cada resta
void matrix_return_edges_savings(void *vm, int sizeVertex, void *vk, void *vn);

// Retorna o custo de uma rota
double matrix_return_route_cost(void *vm, int *route, int size_route);

// Desaloca a matriz de adjacências da memória
void matrix_destroy(void *vm, int size);

#endif