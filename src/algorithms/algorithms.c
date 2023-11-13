#include "algorithms.h"
#include "../graphviz_print/graphviz_print.h"
#include "../union_find/union_find.h"
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

void clarke_wright_serial_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges){

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);
    qsort(near_0, graph_return_num_vertex(g)-1, sizeof(Edges), edges_compare_growing);

    int num_vertex = graph_return_num_vertex(g),
        num_trucks = graph_return_trucks(g),
        size_act_route = 0, size_gl_route = 1,
        capacity = graph_return_capacity(g),
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

    for(int i = 0; i < num_trucks; i++){

        demand_act_route = 0;
        size_act_route = 0;
        control = 0;

        for(int j = 0; j < sizeEdges; j++){

            if( !size_act_route ){
            // SE FOR PRIMEIRO ELEMENTO DA ROTA, PEGA O + PROX DO DEPOSITO //

                for(int k = 0; k < num_vertex - 1; k++){
                    if( !gl_visited[near_0[k].dest] ){
                        act_route[size_act_route++] = near_0[k].dest;
                        demand_act_route = demands[near_0[k].dest];
                        gl_visited[near_0[k].dest] = 1;
                        control = 1;
                        j = -1;
                        break;
                    }
                }
            
            } else if( act_route[0] == e[j].dest || act_route[0] == e[j].src ) {
            // SE O DEST ESTIVER NO INICIO, ADICIONA O SRC NO INICIO DA ROTA //

                int search = ( act_route[0] == e[j].dest ) ? e[j].src : e[j].dest;
                if( demand_act_route + demands[search] <= capacity && !gl_visited[search] ){
                    
                    demand_act_route += demands[search];
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
                if( demand_act_route + demands[search] <= capacity && !gl_visited[search] ){

                    demand_act_route += demands[search];
                    gl_visited[search] = 1;

                    act_route[size_act_route++] = search;
                    control = 1;
                }
            }

            if( j == sizeEdges - 1 ){
                for(int d = 1; d < num_vertex; d++){
                    if( !gl_visited[d] && demand_act_route + demands[d] <= capacity ){
                        j = -1;
                        break;
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

    } // Fim for trucks

    free(gl_visited);
    free(act_route);
    free(demands);
}

void clarke_wright_paralel_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges){

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

    int ver = 0, rot = 0;


    while( noVisiteds ){

        for( int i = 0; i < sizeEdges; i++){
            if(!i) ver++;

    // if( demands[e[i].src] < capacity)
    // printf("uai\n");

            for(int j = 0; j < num_trucks; j++){
                if(!j) rot++;

                if( !size[j] ){
                    if( !visited[e[i].src] && !visited[e[i].dest] && (demands[e[i].src] + demands[e[i].dest]) <= capacity ){

                        demand_route[j] = demands[e[i].src] + demands[e[i].dest];
                        visited[e[i].src] = visited[e[i].dest] = 1;

                        route[j][size[j]++] = e[i].src;
                        route[j][size[j]++] = e[i].dest;

                        noVisiteds -= 2;

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

                        break;

                    }
                }

                if( ver == 4 ){
                    for(int v = 1; v < num_vertex; v++){
                        if(!visited[v]){
                            char control = 0;
                            for(int sz = 0; sz < num_trucks; sz++){
                                if( size[sz] == 0 ){
                                    route[sz][size[sz++]] = v;
                                    demand_route[sz] = demands[v];
                                    visited[v] = 1;
                                    noVisiteds--;
                                    i = -1;
                                    j = num_trucks + 1;
                                    control = 1;
                                }
                            }
                            if( control ) break;

                            num_trucks++;
                            graph_set_trucks(g, num_trucks);

                            route = realloc(route, sizeof(int*) * num_trucks);
                            size = realloc(size, sizeof(int) * num_trucks);
                            demand_route = realloc(demand_route, sizeof(float) * num_trucks);
                            route[num_trucks - 1] = malloc(sizeof(int) * num_vertex);

                            demand_route[num_trucks - 1] = demands[v];
                            size[num_trucks - 1] = 1;
                            visited[v] = 1;
                            route[num_trucks - 1][size[num_trucks - 1] - 1] = v;

                            noVisiteds--;
                            i = -1;
                            j = num_trucks + 1;
                            break;
                        }
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

    free(visited);
    free(size);
    free(demand_route);
    free(demands);
    for(int i = 0; i < num_trucks; i++)
        free(route[i]);
    free(route);
}

void _reverse_route(int lo, int hi, int *route){
    while( hi > lo ){
        int aux = route[hi];
        route[lo] = route[hi] ^ route[lo];
        route[hi] = route[hi] ^ route[lo];
        route[lo] = route[hi] ^ route[lo];
        hi--;
        lo++;
    }
}

void opt2_algorithm(int *route, int sizeRoute, void *graph_adj){
    char improved = 1;

    while( improved ){
        improved = 0;
        sizeRoute -= 2;
        for(int i = 0; i < sizeRoute; i++){
            for(int j = i + 1; j < sizeRoute; j++){
                int vi1 = route[i], vi2 = route[i+1],
                    vj1 = route[j], vj2 = route[(j+1) % (sizeRoute+1)];
                weight cur_weight = matrix_return_edge_weight(graph_adj, vi1, vi2, UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, vj1, vj2, UNDIRECTED);

                weight new = matrix_return_edge_weight(graph_adj, vi1, vj1, UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, vi2, vj2, UNDIRECTED);
                if(new < cur_weight){
                    improved = 1;
                    _reverse_route(i+1, j, route);

                }
            } // Fim for j
        } // Fim for i
    } // Fim while
}

void _merge(Graph *g, int *a, int *aux, int lo, int mid, int hi) {
    for (int k = lo; k <= hi; k++)
        aux[k] = a[k]; // Copy array
    int i = lo, j = mid+1;
    for (int k = lo; k <= hi; k++) { // Merge
        if (i > mid) a[k] = aux[j++];
        else if (j > hi) a[k] = aux[i++];
        else if ( route_return_demand(g, aux[j]) < route_return_demand(g, aux[i]) ) a[k] = aux[j++];
        else a[k] = aux[i++];
    }
}
#define SZ2 (sz+sz)
#define MIN(X,Y) ((X < Y) ? (X) : (Y))
void _sort(Graph *g, int *a, int lo, int hi) {
    int N = (hi - lo) + 1;
    int y = N - 1;
    int *aux = malloc(N * sizeof(int));
    for (int sz = 1; sz < N; sz = SZ2) {
        for (int lo = 0; lo < N-sz; lo += SZ2) {
            int x = lo + SZ2 - 1;
            _merge(g, a, aux, lo, lo+sz-1, MIN(x,y));
        }
    }
    free(aux);
}

char demandFits(Data *d1, Data *d2, Graph *g, int idx){
    if( route_return_demand(g, idx) - data_return_demand(d2) + data_return_demand(d1)
        <= graph_return_capacity(g) ){
            return 1;
    }
    return 0;
}

void _route_swap(int *route1, int v1, int *route2, int v2){
    route1[v1] = route1[v1] ^ route2[v2];
    route2[v2] = route1[v1] ^ route2[v2];
    route1[v1] = route1[v1] ^ route2[v2];
}

void _route_remove(int *route, int v, int *size){

    for(int i = v; i < *size - 1; i++){
        route[i] = route[i + 1];
    }
    *size--;
}

void vns_algorithm(Graph *g, int *demands){
    _sort(g, demands, 0, graph_return_trucks(g));
    for(int i = 0; i < graph_return_trucks(g); i++)
        printf("%d ,", route_return_demand(g, demands[i]));

    int numTrucks, realTrucks = graph_return_trucks(g),
        **route = malloc(sizeof(int*) * realTrucks),
        *route_size = malloc(sizeof(int) * realTrucks);
    char *name = graph_return_name(g);
    sscanf(name, "%*[^k]k%d" , &numTrucks);

    for(int i = 0; i < realTrucks; i++){
        route[i] = malloc(sizeof(int) * graph_return_num_vertex(g));
        memcpy(route[i], route_return_route(g, i), route_return_size(g, i) + 1);
        route_size[i] = route_return_size(g, i);
    }

    while( realTrucks > numTrucks ){
        int k = 0;
        _sort(g, demands, 0, graph_return_trucks(g) - 1);

        while( k < realTrucks - 2 ){

            for( int i = 1; i < route_size[k] - 1; i++){
                Data *d1 = vector_get(graph_return_vertex_vector(g), route[k][i]);
                for(int j = 1; j < route_size[k + 1] - 1; j++){
                    Data *d2 = vector_get(graph_return_vertex_vector(g), route[k + 1][j]); 
                    if( data_return_demand(d2) < data_return_demand(d1) && demandFits(d1, d2, g, k + 1)){
                        _route_swap(route[k], i, route[k + 1], j);
                    }
                }
            }
        }
        Data *left;
        for(int i = 0; i < realTrucks - 1; i++){
            for(int j = 1; j < route_size[realTrucks - 1] - 1; j++){
                left = vector_get(graph_return_vertex_vector(g), route[realTrucks - 1][j]);
                if( route_return_demand(g, i) + data_return_demand(left) <= graph_return_capacity(g) )
                    route[i][--route_size[i]] = route[realTrucks - 1][j];
                    route_size[i]++;
                    route[i][route_size[i]++] = 0;
                    _route_remove(route[realTrucks + 1], route[realTrucks - 1][j], &route_size[realTrucks + 1]);
            }
        }
        if( route_size[realTrucks - 1] == 2 ){
            free(route[realTrucks - 1]);
            realTrucks--;
        }
    }
}