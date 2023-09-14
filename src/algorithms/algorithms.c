#include "algorithms.h"
#include "../union_find/union_find.h"

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