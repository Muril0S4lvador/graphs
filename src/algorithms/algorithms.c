#include "algorithms.h"
#include "../graphviz_print/graphviz_print.h"
#include "../union_find/union_find.h"
#include "../adjacency_list/list.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"

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

char _find_in_route(int *route, int size, Edges e){
    for(int i = 1; i < size - 1; i++)
        if( route[i] == e.src || route[i] == e.dest ) return 1;
    return 0;
}

void clarke_wright_algorithm(Graph *g, Edges *e, int sizeEdges){

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);
    
    int size_act_route = 0, size_gl_route = 1,
        demand_act_route = 0,
        capacity = graph_return_capacity(g),
        trucks = graph_return_trucks(g),
        *act_route = malloc(sizeof(int) * graph_return_num_vertex(g) ),
        *global_route = malloc(sizeof(int) * ((graph_return_num_vertex(g) * 2) + 1) );

    float *demands = malloc(sizeof(float) * graph_return_num_vertex(g));
    char rest1 = 0, rest2 = 0;

    *(act_route) = *(global_route) = 0;

    int size = graph_return_num_vertex(g);
    for(int i = 0; i < size; i++){
        Data *d = vector_get(graph_return_vertex_vector(g), i);
        demands[i] = data_return_demand(d);
    }

    for(int i = 0; i < trucks; i++){

        demand_act_route = 0;
        size_act_route = 0;

        for(int j = 0; j < sizeEdges; j++){

            //se nao esta em uma rota ja feita
            if( !_find_in_route(global_route, size_gl_route, e[j]) ){

                // se for o primeiro elemento da rota
                if( !size_act_route ){
                /* verifica demanda e add */ 

                
                // se for um elemento no final da rota
                } else if( act_route[0] == e[j].dest ) {
                /* verifica demanda e add */ 


                // se for um elemento no inicio da rota
                } else if( act_route[size_act_route - 1] == e[j].src ){
                /* verifica demanda e add */ 

                
                }
            }

            // Garante que não é alguém do meio do vetor ou ja foi visto em outras rotas anteriores
            if( !_find_in_route(act_route, size_act_route, e[i]) && !_find_in_route(global_route, size_gl_route, e[i]) ){

                int dest = -1, src = -1, brek = 0;
                if( act_route[0] == e[i].src ) brek = 1;
                else if( act_route[0] == e[i].dest ) {src = 1; dest = 0}
                else if( act_route[size_act_route-1] == e[i].dest ) brek = 1;
                else if( act_route[size_act_route-1] == e[i].src ) {dest = 1; src = 0}

                if(brek) break;

                float demand = 0;
                if( dest == src && dest == -1 ){
                    demand = demands[e[j].src] + demands[e[j].dest];

                } else if( dest == 1 ){
                    demand = demands[e[j].dest];

                } else if(src == 1){
                    demand = demands[e[j].src];

                }

                if( demand_act_route += demand <= capacity ){

                    if( src && !dest ){
                        // adiciona inicio vetor


                    } else if( !src && dest ){
                        // adiciona final vetor


                    } else if( src && dest ){
                        // adiciona os dois no vetor (caso inicial)


                    }
                }
            }
        }

        // add rota atual na global
        int k = size_gl_route, l = 0;
        size_gl_route += size_act_route + 2;
        for( k, l ; l < size_act_route; l++, k++){

            if( l < size_act_route )
                global_route[k] = act_route[l];

        }
        global_route[k++] = 0;

    }
}