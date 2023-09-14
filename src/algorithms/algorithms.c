#include "algorithms.h"

Graph *kruskal_algorithm(Graph *g){

    const int sizeVertex = graph_return_num_vertex(g), maxSizeEdges = ((sizeVertex * sizeVertex - sizeVertex) / 2);
    Kruskal *k = malloc( maxSizeEdges * sizeof(Kruskal) );

    for(int i = 0; i < maxSizeEdges; i++){
        
    }




}