#ifndef _UF_H_
#define _UF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint16_t N;     // Number of vertices
    uint16_t *id;   // id[i] = parent of i
    uint16_t *size; // size[i] = number of vertices in subtree rooted at i
} UF;

/// @brief Initialize the Union-Find structure
/// @param N Number of vertices
/// @return UF* Pointer to the Union-Find structure
UF* UF_init(uint16_t N);

/// @brief Find the root of the tree
/// @param uf Pointer to the Union-Find structure
/// @param i Vertex
/// @return uint16_t Root of the tree
uint16_t UF_find(UF* uf, uint16_t i);

/// @brief Union two trees
/// @param uf Pointer to the Union-Find structure
/// @param p Vertex
/// @param q Vertex
void UF_union(UF* uf, uint16_t p, uint16_t q);

/// @brief Check if two vertices are connected
/// @param uf Pointer to the Union-Find structure
/// @param p Vertex
/// @param q Vertex
/// @return int 1 if connected, 0 otherwise
int UF_connected(UF* uf, uint16_t p, uint16_t q);

void UF_free(UF* uf);

#endif