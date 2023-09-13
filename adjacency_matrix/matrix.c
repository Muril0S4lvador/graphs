#include "matrix.h"

Matrix matrix_construct(int v){
    Matrix m = malloc(v * sizeof(int*));

    for(int i = 0; i < v; i++)
        m[i] = calloc(v, sizeof(int));

    return m;
}

void matrix_add_edge(void *vm, int v1, int v2, weight peso){
    if( matrix_edge_exists(vm, v1, v2) ) return;
    Matrix m = vm;
    m[v1][v2] = peso;
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

void matrix_destroy(void *vm, int size){
    Matrix m = vm;
    for(int i = 0; i < size; i++)
        free(m[i]);
    free(m);
}