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

    return (ka->weight - kb->weight)*100 ;
}

int edges_compare_descending(const void* a, const void* b) {
    Edges* ka = (Edges*) a;
    Edges* kb = (Edges*) b;

    return (kb->weight - ka->weight)*100 ;
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

void clarke_wright_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges){

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);
    qsort(near_0, graph_return_num_vertex(g)-1, sizeof(Edges), edges_compare_growing);

    int size_act_route = 0, size_gl_route = 1,
        capacity = graph_return_capacity(g),
        trucks = graph_return_trucks(g),
        *act_route = malloc(sizeof(int) * graph_return_num_vertex(g) ),
        *global_route = malloc(sizeof(int) * ((graph_return_num_vertex(g) * 2) + 1) ),
        *gl_visited = calloc(graph_return_num_vertex(g), sizeof(int));

    float *demands = malloc(sizeof(float) * graph_return_num_vertex(g)),
            demand_act_route = 0;
    char control = 0;

    *(act_route) = *(global_route) = 0;


    int size = graph_return_num_vertex(g);
    for(int i = 0; i < size; i++){
        Data *d = vector_get(graph_return_vertex_vector(g), i);
        demands[i] = data_return_demand(d);
    }

    for(int i = 0; i < trucks; i++){

        demand_act_route = 0;
        size_act_route = 0;
        control = 0;
        int *act_visited = calloc(graph_return_num_vertex(g), sizeof(int));

        if( i == 1 )
            i = 4 - 3 * 1;

        for(int j = 0; j < sizeEdges; j++){

            if( !size_act_route ){
            // SE FOR PRIMEIRO ELEMENTO DA ROTA, PEGA O + PROX DO DEPOSITO //

                for(int k = 0; k < graph_return_num_vertex(g)-1; k++){
                    if( !gl_visited[near_0[k].dest] ){
                        act_route[size_act_route++] = near_0[k].dest;
                        demand_act_route = demands[near_0[k].dest];
                        act_visited[near_0[k].dest] = 1;
                        gl_visited[near_0[k].dest] = 1;
                        control = 1;
                        j = -1;
                        break;
                    }
                }
            
            } else if( act_route[0] == e[j].dest || act_route[0] == e[j].src ) {
            // SE O DEST ESTIVER NO INICIO, ADICIONA O SRC NO INICIO DA ROTA //

                int search = ( act_route[0] == e[j].dest ) ? e[j].src : e[j].dest;
                if( demand_act_route + demands[search] <= capacity && !act_visited[search] && !gl_visited[search] ){
                    
                    demand_act_route += demands[search];
                    act_visited[search] = 1;
                    gl_visited[search] = 1;

                    int *r = malloc(sizeof(int) * graph_return_num_vertex(g));
                    r[0] = search;
                    for(int c = 1; c <= size_act_route; c++){
                        r[c] = act_route[c-1];
                    }
                    size_act_route++;
                    free(act_route);
                    act_route = r;
                    control = 1;
                }

            } else if( act_route[size_act_route - 1] == e[j].src && act_route[size_act_route - 1] == e[j].dest ){
            // SE ESTIVER NO FINAL, ADICIONA NO FINAL DA ROTA //

                int search = ( act_route[size_act_route - 1] == e[j].dest ) ? e[j].src : e[j].dest;
                if( demand_act_route + demands[search] <= capacity && !act_visited[search] && !gl_visited[search] ){

                    demand_act_route += demands[search];
                    act_visited[search] = 1;
                    gl_visited[search] = 1;

                    act_route[size_act_route++] = search;
                    control = 1;
                }
            }

            if( j == sizeEdges - 1 ){
                int tam = graph_return_num_vertex(g);
                for(int d = 1; d < tam; d++){
                    if( !gl_visited[d] && !act_visited[d] && demand_act_route + demands[d] <= capacity ){
                        j = 0;                        
                    }
                }
            }
        } // Fim for edges e

        if(control){
            int *r = malloc(sizeof(int) * graph_return_num_vertex(g));
            r[0] = 0;
            for(int c = 1; c <= size_act_route; c++){
                r[c] = act_route[c-1];
            }
            size_act_route++;
            free(act_route);
            act_route = r;
            act_route[size_act_route++] = 0;
            graph_set_route(g, i, act_route, size_act_route);
        }

        printf("Rota %d - Demanda = %.2f\n", i, demand_act_route);
        for(int i = 0; i < size_act_route; i++){
            printf("v%d ", act_route[i]);
            if(i < size_act_route -1) printf("-- ");
        }
        printf("\n");

    } // Fim for trucks

    int count = 0;
    for(int i = 1; i < graph_return_num_vertex(g); i++)
        if( !gl_visited[i] )
            printf("%d(%.2f)\n", i, demands[i]);

}