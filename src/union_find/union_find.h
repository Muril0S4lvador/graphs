#ifndef _UF_H_
#define _UF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int N;     // Number of vertices
    int *id;   // id[i] = parent of i
    int *size; // size[i] = number of vertices in subtree rooted at i
} UF;

/// @brief Initialize the Union-Find structure
/// @param N Number of vertices
/// @return UF* Pointer to the Union-Find structure
UF* UF_init(int N);

/// @brief Find the root of the tree
/// @param uf Pointer to the Union-Find structure
/// @param i Vertex
/// @return uint16_t Root of the tree
int UF_find(UF* uf, int i);

/// @brief Union two trees
/// @param uf Pointer to the Union-Find structure
/// @param p Vertex
/// @param q Vertex
void UF_union(UF* uf, int p, int q);

/// @brief Check if two vertices are connected
/// @param uf Pointer to the Union-Find structure
/// @param p Vertex
/// @param q Vertex
/// @return int 1 if connected, 0 otherwise
int UF_connected(UF* uf, int p, int q);

void UF_free(UF* uf);

#endif