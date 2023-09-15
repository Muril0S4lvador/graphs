#include "algorithms.h"
#include "../union_find/union_find.h"
#include "../adjacency_list/list.h"

Graph *kruskal_algorithm(Kruskal *k, int num_vertex, int num_edges){

    UF* uf = UF_init(num_vertex);
    Graph *mst = graph_construct(num_vertex, DIRECTED);
    
    qsort(k, num_edges, sizeof(Kruskal), kruskal_compare);

    Kruskal *current_k = k;
    for(int i = 0; i < num_edges; i++) {
        int u = UF_find(uf, current_k->src);
        int v = UF_find(uf, current_k->dest);

        if(!UF_connected(uf, u, v)) {
            UF_union(uf, u, v);
            graph_add_edge(mst, current_k->src, current_k->dest, current_k->weight);
            graph_add_edge(mst, current_k->dest, current_k->src, current_k->weight);
        }
        current_k++;
    }

    UF_free(uf);
    free(k);
    return mst;
}

int kruskal_compare(const void* a, const void* b) {
    Kruskal* ka = (Kruskal*) a;
    Kruskal* kb = (Kruskal*) b;

    return (ka->weight - kb->weight) ;
}

void dfs_algorithm(void *adj, int *route, int *size_route, int *visited){

    if( MATRIX ){
        

    } else if( LIST ){
        list_dfs_recursive(adj, route, size_route, visited);
        
    }

    printf("\nRoute\n");
    for(int i = 0; i < *size_route; i++){
        printf("%d\n", route[i]);

    }

}