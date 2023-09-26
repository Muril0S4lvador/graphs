#include "matrix.h"
#include "../algorithms/algorithms.h"

Matrix matrix_construct(int v){
    Matrix m = malloc(v * sizeof(int*));

    for(int i = 0; i < v; i++)
        m[i] = calloc(v, sizeof(int));

    return m;
}

char matrix_add_edge(void *vm, int v1, int v2, weight peso){
    if( matrix_edge_exists(vm, v1, v2) ) return 0;
    Matrix m = vm;
    m[v1][v2] = peso;
    return 1;
}

void matrix_remove_edge(void *vm, int v1, int v2){
    if( !matrix_edge_exists(vm , v1, v2) ) return;
    Matrix m = vm;
    m[v1][v2] = 0;
}

char matrix_edge_exists(void *vm, int v1, int v2){
    Matrix m = vm;
    return m[v1][v2] != 0;
}

void matrix_print(void *vm, int size){
    Matrix m = vm;
    printf("-- ");
    for(int i = 0; i < size; i++)
        printf("v%d ", i);

    for(int i = 0; i < size; i++){
        printf("\n");
        printf("v%d:", i);

        for(int j = 0; j < size; j++)
            printf(" %d", m[i][j]);

    }
    printf("\n");
}

void matrix_file_write(void *vm, int size, FILE *arq, char *edge){
    Matrix m = vm;
    for(int i = 0; i < size; i++)
        for( int j = 0; j < size; j++)
            if( m[i][j] )
                fprintf(arq, "v%d %s v%d [label = %c%.2d%c];\n", i, edge, j, '"', m[i][j], '"');

}

void matrix_return_kruskal(void *vm, int sizeVertex, int sizeEdges, void *vk, int direction){
    Matrix m = vm;
    Kruskal *k = vk;

    for(int i = 0, k_size = 0; i < sizeVertex; i++){

        int j = ( direction == UNDIRECTED ) ? i + 1 : 0;
        for( j; j < sizeVertex; j++){

            k[k_size].src = i;
            k[k_size].dest = j;
            k[k_size].weight = m[i][j];
            k_size++;

        }
    }
}

void matrix_dfs_recursive(void *vm, int *route, int *size_route, int *visited, int size){
    Matrix m = vm;
    route[(*size_route)++] = 0;
    visited[0] = 1;
    _matrix_dfs_recursive(vm, 0, 0, route, size_route, visited, size);
}

void _matrix_dfs_recursive(void *vm, int i, int j, int *route, int *size_route, int *visited, int size){
    Matrix m = vm;
    // if( i >= size ) return;

    if( j >= size  ) { return; }
    if( m[i][j] && !visited[j] ){
        route[(*size_route)++] = j;
        visited[j] = 1;
        _matrix_dfs_recursive(vm, j, 0, route, size_route, visited, size);
    }
    _matrix_dfs_recursive(vm, i, j+1, route, size_route, visited, size);

}

void matrix_destroy(void *vm, int size){
    Matrix m = vm;
    for(int i = 0; i < size; i++)
        free(m[i]);
    free(m);
}