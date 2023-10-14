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

void clarke_wright_serial_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges){

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);
    qsort(near_0, graph_return_num_vertex(g)-1, sizeof(Edges), edges_compare_growing);

    int num_vertex = graph_return_num_vertex(g),
        num_trucks = graph_return_trucks(g),
        size_act_route = 0, size_gl_route = 1,
        capacity = graph_return_capacity(g),
        trucks = num_trucks,
        *act_route = malloc( sizeof(int) * num_vertex ),
        *gl_visited = malloc( sizeof(int) * num_vertex );

    float *demands = malloc(sizeof(float) * num_vertex),
            demand_act_route = 0;
    char control = 0;

    *(act_route) = 0;

    for(int i = 0; i < num_vertex; i++){
        gl_visited[i] = 0;
        Data *d = vector_get(graph_return_vertex_vector(g), i);
        demands[i] = data_return_demand(d);
    }

    for(int i = 0; i < trucks; i++){

        demand_act_route = 0;
        size_act_route = 0;
        control = 0;
        int *act_visited = calloc(num_vertex, sizeof(int));

        for(int j = 0; j < sizeEdges; j++){

            if( !size_act_route ){
            // SE FOR PRIMEIRO ELEMENTO DA ROTA, PEGA O + PROX DO DEPOSITO //

                for(int k = 0; k < num_vertex - 1; k++){
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

                    int *r = malloc(sizeof(int) * num_vertex);
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
                for(int d = 1; d < num_vertex; d++){
                    if( !gl_visited[d] && !act_visited[d] && demand_act_route + demands[d] <= capacity ){
                        j = 0;                        
                    }
                }
            }
        } // Fim for edges e

        if(control){
            int *r = malloc(sizeof(int) * num_vertex);
            r[0] = 0;
            for(int c = 1; c <= size_act_route; c++){
                r[c] = act_route[c-1];
            }
            size_act_route++;
            free(act_route);
            act_route = r;
            act_route[size_act_route++] = 0;
            graph_set_route(g, i, act_route, size_act_route, demand_act_route);
        }

        printf("Rota %d - Demanda =\n", i);
        for(int i = 0; i < size_act_route; i++){
            printf("v%d ", act_route[i]);
            if(i < size_act_route -1) printf("-- ");
        }
        printf("\n");

        free(act_visited);
    } // Fim for trucks

    int count = 0;
    for(int i = 1; i < num_vertex; i++)
        if( !gl_visited[i] )
            printf("%d(%.2f)\n", i, demands[i]);
    free(gl_visited);
    free(act_route);
    free(demands);

}

void clarke_wright_paralel_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges){

    // Pegar a menor edge e, conferir se posso adicionar na atual, 
    // se puder, beleza.
    // Caso não, verifico se posso adicionar na próxima.
    // Caso nao posso adicionar em nenhuma, passo p proxima edge e.

    // matriz de rotas atuais, cada linha uma rota diferente, cada coluna um vertice.

    // 1 vetor de vertice, so marca positivo se estiver "dentro" do vetor(nem no inicio nem no ultimo)

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);

   int num_vertex = graph_return_num_vertex(g),        // Número de vértices no grafo
        num_trucks = graph_return_trucks(g),           // Número de caminhões no grafo
        **route = malloc(sizeof(int*) * num_trucks),   // Matriz de rotas
        *visited = malloc(sizeof(int) * num_vertex),   // Vetor bool se vértice foi visitado ou não
        noVisiteds = num_vertex - 1,                   // Número de vértices não visitados
        *size = malloc(sizeof(int) * num_trucks),      // Tamanho de cada rota
        capacity = graph_return_capacity(g);           // Capacidade dos caminhões

    float *demands = malloc(sizeof(float) * num_vertex),              // Vetor de demanda de todos os vertices
            *demand_route = malloc(sizeof(float) * num_trucks);       // vetor de demanda das rotas

    // Inicializa elementos de rota
    for( int i = 0; i < num_trucks; i++){
        route[i] = malloc(sizeof(int) * num_vertex);
        size[i] = demand_route[i] = 0;
    }

    // Preenche vetor de demanda dos vértices
    for(int i = 0; i < num_vertex; i++){   
        visited[i] = 0; 
        Data *d = vector_get(graph_return_vertex_vector(g), i);
        demands[i] = data_return_demand(d);
    }
    
    while( noVisiteds ){
        
        for( int i = 0; i < sizeEdges; i++){
            printf("%d \n", noVisiteds);

            for(int j = 0; j < num_trucks; j++){

                if( !size[j] ){
                    if( !visited[e[i].src] && !visited[e[i].dest] && (demands[e[i].src] + demands[e[i].dest]) <= capacity ){

                        demand_route[j] = demands[e[i].src] + demands[e[i].dest];
                        visited[e[i].src] = visited[e[i].dest] = 1;

                        route[j][size[j]++] = e[i].src;
                        route[j][size[j]++] = e[i].dest;

                        noVisiteds -= 2;

printf("%d -- %d(%.1f) Add em R%d\n", e[i].src, e[i].dest, e[i].weight, j);

                        break;

                    }

                } else if( route[j][0] == e[i].dest || route[j][0] == e[i].src ) {
                // SE O DEST ESTIVER NO INICIO, ADICIONA O SRC NO INICIO DA ROTA //

                    int search = ( route[j][0] == e[i].dest ) ? e[i].src : e[i].dest;
                    if( demand_route[j] + demands[search] <= capacity && !visited[search] ){

                        demand_route[j] += demands[search];
                        visited[search] = 1;

                        for(int c = size[j] - 1; c >= 0 && c < num_vertex - 1; c--){
                            route[j][c + 1] = route[j][c];
                        }

printf("%d -- %d Add em R%d\n", e[i].src, e[i].dest, j);


                        route[j][0] = search;
                        size[j]++;
                        noVisiteds--;
                        break;
                    }
                    // confere e add

                } else if( route[j][size[j] - 1] == e[i].src || route[j][size[j] - 1] == e[i].dest ){
                // SE ESTIVER NO FINAL, ADICIONA NO FINAL DA ROTA //

                    int search = ( route[j][size[j] - 1] == e[i].dest ) ? e[i].src : e[i].dest;
                    if( demand_route[j] + demands[search] <= capacity && !visited[search] ){

                        demand_route[j] += demands[search];
                        visited[search] = 1;

                        route[j][size[j]++] = search;
                        noVisiteds--;

printf("%d -- %d Add em R%d\n", e[i].src, e[i].dest, j);

                        
                        break;

                    }
                }

            } // Fim for trucks

        } // Fim for Edges

    } // Fim while non_visited

    for( int i = 0; i < num_trucks; i++ ){

        for(int j = size[i] - 1; j >= 0 && j < num_vertex - 1; j--){
            route[i][j + 1] = route[i][j];
        }
        route[i][0] = 0;
        size[i]++;
        route[i][size[i]++] = 0;

        graph_set_route(g, i, route[i], size[i], demand_route[i]);

    }

}

/*
A-n32-k5
32
100
Rota 0
0 26 5 21 10 31 24 19 30 0 Rota 0 - Demanda = 214.32
v0 -- v26 -- v5 -- v21 -- v10 -- v31 -- v24 -- v19 -- v30 -- v0
Rota 1
0 22 29 7 14 13 15 17 16 0 Rota 1 - Demanda = 403.19
v0 -- v22 -- v29 -- v7 -- v14 -- v13 -- v15 -- v17 -- v16 -- v0
Rota 2
0 12 25 1 9 27 0 Rota 2 - Demanda = 146.41
v0 -- v12 -- v25 -- v1 -- v9 -- v27 -- v0
Rota 3
0 3 11 28 4 6 23 8 18 20 0 Rota 3 - Demanda = 1074.64
v0 -- v3 -- v11 -- v28 -- v4 -- v6 -- v23 -- v8 -- v18 -- v20 -- v0
Rota 4
0 2 0 Rota 4 - Demanda = 155.76
v0 -- v2 -- v0
*/