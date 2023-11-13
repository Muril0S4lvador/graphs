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

float matrix_return_edge_weight(void *vm, int v1, int v2, int direction){
    if( direction == UNDIRECTED )if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }
    Matrix m = vm;
    return m[v1][v2];
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
            printf(" %2.0f", m[i][j]);

    }
    printf("\n");
}

void matrix_file_write(void *vm, int size, FILE *arq, char *edge){
    Matrix m = vm;
    for(int i = 0; i < size; i++)
        for( int j = 0; j < size; j++)
            if( m[i][j] )
                fprintf(arq, "v%d %s v%d [label = %c%.2f%c];\n", i, edge, j, '"', m[i][j], '"');

}

void matrix_return_edges(void *vm, int sizeVertex, void *vk, int direction){
    Matrix m = vm;
    Edges *k = vk;

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

void matrix_return_edges_savings(void *vm, int sizeVertex, void *vk, void *vn){
    Matrix m = vm;
    Edges *e = vk,
          *near_0 = vn;
    int sizeE = 0;

    for(int i = 1; i < sizeVertex; i++){
        near_0[i-1].src = 0;
        near_0[i-1].dest = i;
        near_0[i-1].weight = m[0][i];

        for(int j = i + 1; j < sizeVertex; j++){

            if( m[i][j] && m[0][i] && m[0][j] ){

                e[sizeE].src = i;
                e[sizeE].dest = j;
                e[sizeE].weight = m[0][i] + m[0][j] - m[i][j];
                sizeE++;
            }
        }
    }
}

float matrix_return_route_cost(void *vm, int *route, int size_route){
    Matrix m = vm;
    float cost = 0;

    for(int i = 0; i < size_route - 1; i++){
        cost += matrix_return_edge_weight(vm, route[i], route[i+1], UNDIRECTED);
    }

    return cost;
}

void matrix_destroy(void *vm, int size){
    Matrix m = vm;
    for(int i = 0; i < size; i++)
        free(m[i]);
    free(m);
}