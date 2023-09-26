#include "algorithms.h"
#include "../graphviz_print/graphviz_print.h"
#include "../union_find/union_find.h"
#include "../adjacency_list/list.h"
#include "../adjacency_matrix/matrix.h"

Graph *kruskal_algorithm(Edges *k, int num_vertex, int num_edges, Graph *g){

    UF* uf = UF_init(num_vertex);
    Graph *mst = graph_construct(num_vertex, UNDIRECTED);
    
    qsort(k, num_edges, sizeof(Edges), edges_compare_growing);

    Edges *current_k = k;
    for(int i = 0, j = 0; i < num_edges; i++) {
        int u = UF_find(uf, current_k->src);
        int v = UF_find(uf, current_k->dest);

        if(!UF_connected(uf, u, v)) {
            UF_union(uf, u, v);
            graph_add_edge(mst, current_k->src, current_k->dest, current_k->weight);
            img_print_graph_per_edge(g, mst, j++, "imgs/mst");

        }
        current_k++;
    }

    system("mv imgs/*.dot imgs/arq_dots/");

    UF_free(uf);
    free(k);
    return mst;
}

int edges_compare_growing(const void* a, const void* b) {
    Edges* ka = (Edges*) a;
    Edges* kb = (Edges*) b;

    return (ka->weight - kb->weight) ;
}

int edges_compare_descending(const void* a, const void* b) {
    Edges* ka = (Edges*) a;
    Edges* kb = (Edges*) b;

    return (kb->weight - ka->weight) ;
}

void dfs_algorithm(void *adj, int *route, int *visited, int size){

    int *size_route = malloc(sizeof(int));
    *size_route = 0;

    if( MATRIX ){
        matrix_dfs_recursive(adj, route, size_route, visited, size);

    } else if( LIST ){
        list_dfs_recursive(adj, route, size_route, visited);
        
    }

    printf("\nRoute\n");
    for(int i = 000; i < *size_route; i++){
        printf("%d\n", route[i]);

    }
    free(size_route);
}

Graph *clarke_wright_algorithm(Graph *g, Edges *e, int sizeEdges){

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);
    
    Graph *cw = graph_construct(graph_return_num_vertex(g), DIRECTED);

    //algoritmo

}