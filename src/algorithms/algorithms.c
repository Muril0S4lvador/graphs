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
        *gl_visited = calloc( num_vertex,  sizeof(int) );

    float *demands = graph_return_demands(g),
            demand_act_route = 0;
    char control = 0;

    *(act_route) = 0;

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

void clarke_wright_parallel_algorithm(Graph *g, Edges *e, Edges *near_0, int sizeEdges){

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);

   int num_vertex = graph_return_num_vertex(g),        // Número de vértices no grafo
        num_trucks = graph_return_trucks(g),           // Número de caminhões no grafo
        **route = malloc(sizeof(int*) * num_trucks),   // Matriz de rotas
        *visited = calloc( num_vertex, sizeof(int) ),        // Vetor bool se vértice foi visitado ou não
        noVisiteds = num_vertex - 1,                   // Número de vértices não visitados
        *size = malloc(sizeof(int) * num_trucks),      // Tamanho de cada rota
        capacity = graph_return_capacity(g);           // Capacidade dos caminhões

    float *demands = graph_return_demands(g),                         // Vetor de demanda de todos os vertices
          *demand_route = malloc(sizeof(float) * num_trucks);         // vetor de demanda das rotas

    char imp;

    // Inicializa elementos de rota
    for( int i = 0; i < num_trucks; i++){
        route[i] = malloc(sizeof(int) * num_vertex);
        size[i] = demand_route[i] = 0;
    }

    while( noVisiteds ){

        imp = 0;
        for( int i = 0; i < sizeEdges; i++){

            for(int j = 0; j < num_trucks; j++){

                if( !size[j] ){
                    if( !visited[e[i].src] && !visited[e[i].dest] && (demands[e[i].src] + demands[e[i].dest]) <= capacity ){

                        demand_route[j] = demands[e[i].src] + demands[e[i].dest];
                        visited[e[i].src] = visited[e[i].dest] = 1;

                        route[j][size[j]++] = e[i].src;
                        route[j][size[j]++] = e[i].dest;

                        noVisiteds -= 2;
                        imp = 1;

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
                        imp = 1;

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
                        imp = 1;

                        break;

                    }
                }

                if( imp == 0 ){
                    for(int v = 1; v < num_vertex; v++){
                        if(!visited[v]){

                            demand_route[num_trucks - 1] += demands[v];
                            visited[v] = 1;

                            route[num_trucks - 1][size[num_trucks - 1]++] = v;

                            noVisiteds--;
                        }
                    }
                    i = sizeEdges + 1;
                    j = num_trucks + 1;
                    break;
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

void _swap_local_inRoute(int *local_inRoute, int idx1, int idx2){
    local_inRoute[idx2] = local_inRoute[idx1] ^ local_inRoute[idx2];
    local_inRoute[idx1] = local_inRoute[idx1] ^ local_inRoute[idx2];
    local_inRoute[idx2] = local_inRoute[idx1] ^ local_inRoute[idx2];
}

void _reverse_route(int lo, int hi, int *route){
    while( hi > lo ){
        _swap_local_inRoute(route, hi, lo);
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

/*
    PRECISO
 * Diminuir demanda de rotas fora do limite
 * Reajustar as rotas de modo a menor distância
*/ 

/* ----- ALGORITMOS ----- */

/*
     * ILS:
    S0 <- Gera solução inicial
    S* <- Local Search(S0)
    Enquanto (_CRITERIO_)
        s <- Perturba (Troca com elemento de outra rota)
        s' <- LocalSearch(s)
        S* <- AcceptanceCriterion(S*, s', history)

    Condição de Perturbação: adicionar/trocar itens entre rotas com demanda <= capacidade
    Condição de Aceitação: Custo total de TODAS as rotas tenha diminuido
    Caso tenha acontecido um número alto de iterações(definir) pare o algoritmo
*/

/*
    variable_Neighborhood_Descent (VND):
    S0: Solução inicial;
    r : Número de estruturas de vizinhança.

    S <- S0
    do{
        k <- 1 ( Vizinhança atual )
        PARE <- 0
        S' <- S ( Cópia da solução atual )

        do{
            S" <- MelhorVizinho(S, k)
            AltereVizinhanças(S, S", k)
        } while ( k == r )
            se Custo(S) <= Custo(S')
                PARE <- 1
    } while ( PARE )
    return s'
*/

void _swap_elements(int *route, int sizeRoute, int old_v, int new_v){
    for(int i = 0; i < sizeRoute; i++)
        if(route[i] == old_v)
            route[i] = new_v;
}

int *_route_add_vertex(int *route, int size, int v){
    int *new_route = malloc( sizeof(int) * (size + 1) );
    for(int idx_r = 0, idx_n = 0; idx_r < size; idx_r++, idx_n++)
    {
        if( idx_n == 1 )
        {
            new_route[idx_n] = v;
            idx_r--;
        }
        else
            new_route[idx_n] = route[idx_r];
    }
    return new_route;
}

int *_route_delete_vertex(int *route, int size, int v){
    int *new_route = malloc(sizeof(int) * (size - 1) );
    // printf("Desejado: %d", v);
    for(int idx_r = 0, idx_n = 0; idx_r < size; idx_r++, idx_n++)
    {
        if(route[idx_r] == v)
            idx_n--;
        else
            new_route[idx_n] = route[idx_r];
    }
    // free(route);

    // printf("\nOld ROUTE\n");
    // for(int i = 0; i < size; i++){
    //     printf("%d ", route[i]);
    // }
    // printf("\nnew ROUTE\n");
    // for(int i = 0; i < (size - 1); i++){
    //     printf("%d ", new_route[i]);
    // }
    // printf("\n");
    // exit(0);
    
    return new_route;
}

int **_copy_route_matrix(int **dest, int **src, int size, int *sizeRoute){
    char fre = 1;
    if( dest == NULL ) { dest = malloc(sizeof(int*) * size); fre = 0; }
    for(int i = 0; i < size; i++){
        // if( fre ) free(dest[i]); 
        dest[i] = malloc(sizeof(int) * sizeRoute[i]);
        memcpy(dest[i], src[i], sizeof(int) * sizeRoute[i]);
    }
     if(!fre) return dest;

    printf("\nSRC:\n");
     for(int i = 0; i < size; i++){
        printf("\nRoute %d : ", i);
        for(int j = 0; j < sizeRoute[i]; j++){
            printf("%d ", src[i][j]);
        }
     }

     printf("\nDEST:");
     for(int i = 0; i < size; i++){
        printf("\nRoute %d : ", i);
        for(int j = 0; j < sizeRoute[i]; j++){
            printf("%d ", dest[i][j]);
        }
     }
     printf("\n");
     return dest;
}

int** first_improvement(Graph *g, int **routes, int *sizeRoute, float *demands, int k){
    
    int num_trucks = graph_return_trucks(g),
        num_vertex = graph_return_num_vertex(g),
        capacity = graph_return_capacity(g),
        **mod_routes = NULL,
        *current_route = NULL,
        *local_inRoute = malloc(sizeof(int) * num_vertex),
        *new_sizeRoute = malloc(sizeof(int) * num_trucks);
    
    float new_cost = 0, old_cost = route_return_total_cost(g),
          *route_demand = malloc(sizeof(int) * num_vertex);

    memcpy(new_sizeRoute, sizeRoute, sizeof(int) * num_trucks);
    mod_routes = _copy_route_matrix(mod_routes, routes, num_trucks, new_sizeRoute);
    current_route = mod_routes[k];

    // exit(0);
    
    for(int i = 0; i < num_trucks; i++){
        route_demand[i] = route_return_demand(g, i);
        for(int j = 1; j < sizeRoute[i]; j++)
            local_inRoute[mod_routes[i][j]] = i;
    }
    
    for(int i = 1; i < num_vertex; i++)
    {
        if( local_inRoute[i] == k ) continue;

        if( route_demand[k] + demands[i] <= capacity )
        {
            // add vertice nessa rota e tira da outra
            current_route = _route_add_vertex(current_route, sizeRoute[k], i);
            mod_routes[local_inRoute[i]] = _route_delete_vertex(mod_routes[local_inRoute[i]], sizeRoute[local_inRoute[i]], i);
            route_demand[k] += demands[i];
            route_demand[local_inRoute[i]] -= demands[i];
            new_sizeRoute[k]++;
            new_sizeRoute[local_inRoute[i]]--;
            local_inRoute[i] = k;
        }
        else
        {
            for(int j = 1; j < sizeRoute[k]; j++)
            {
                if( route_demand[k] + demands[i] - demands[j] <= capacity )
                {
                    // troca vertices das duas rotas
                    current_route[j] = i;
                    route_demand[k] += demands[i] - demands[j];
                    route_demand[local_inRoute[i]] += demands[j] - demands[i];
                    _swap_elements(mod_routes[local_inRoute[i]], new_sizeRoute[local_inRoute[i]], i, j);
                    _swap_local_inRoute(local_inRoute, i, j);
                }
            }
        }

        opt2_algorithm(current_route, sizeRoute[k], graph_return_adjacencies(g));
        for(int r = 0; r < num_trucks; r++)
            new_cost += matrix_return_route_cost(graph_return_adjacencies(g), mod_routes[r], new_sizeRoute[r]);

        if( new_cost < old_cost )
        {
            routes = mod_routes;
            sizeRoute = new_sizeRoute;
            printf("IMPROVED! (%.0f < %.0f)\n", new_cost, old_cost);
            return mod_routes;
            // free(mod_routes);
        }
        else{
            return routes;
        }
    }
}

float _return_total_cost_route(int **routes, int *sizeRoute, int num_trucks, void *graph_adj){
    float sum =0;
    for(int i = 0; i < num_trucks; i++){
        // printf("Route %d : %d -- %d\n", i, routes[i][1], sizeRoute[i]);
        sum += matrix_return_route_cost(graph_adj, routes[i], sizeRoute[i]);
    }
    return sum;
}

// /*
void variable_Neighborhood_Descent(Graph *g, int **route, int *size, float *demandVertices){
    char stop;
    int k, **currentSolution = NULL, **bestSolution = NULL, num_trucks = graph_return_trucks(g);
    float currentCost = 0, bestCost = 0;

    // for(int i = 0; i < num_trucks; i++){
    //     printf("\nRota %d\n", i);
    //     for(int j = 0; j < size[i]; j++)
    //         printf("%d ", route[i][j]);
    // }
    
    currentSolution = _copy_route_matrix(currentSolution, route, num_trucks, size);

    // printf("\n\n\nCurrent:\n");

    // for(int i = 0; i < num_trucks; i++){
    //     printf("\nRota %d\n", i);
    //     for(int j = 0; j < size[i]; j++)
    //         printf("%d ", currentSolution[i][j]);
    // }
    // printf("\n");
    // exit(0);

    do{
        k = 0;
        stop = 0;
        bestSolution = _copy_route_matrix(bestSolution, currentSolution, num_trucks, size);
        printf("Before: %.0f\n", bestCost);
        bestCost = _return_total_cost_route(bestSolution, size, num_trucks, graph_return_adjacencies(g));
        printf("After: %.0f\n", bestCost);

        do{
            currentCost = _return_total_cost_route(currentSolution, size, num_trucks, graph_return_adjacencies(g));

            printf("\nCURRENT(%.3f):", currentCost);
            for(int i = 0; i < num_trucks; i++){
                printf("\nRoute %d(%d) : ", i, route_return_demand(g, i));
                for(int j = 0; j < size[i]; j++){
                    printf("%d ", currentSolution[i][j]);
                }
            }
            
            int **testSolution = first_improvement(g, route, size, demandVertices, k);
            float testCost = _return_total_cost_route(testSolution, size, num_trucks, graph_return_adjacencies(g));

            printf("\nTEST (%.3f):", testCost);
            for(int i = 0; i < num_trucks; i++){
                printf("\nRoute %d(%d) : ", i, route_return_demand(g, i));
                for(int j = 0; j < size[i]; j++){
                    printf("%d ", testSolution[i][j]);
                }
            }

            // if(curr)
            
            currentSolution = neighborhood_Change_Cyclic(currentSolution, testSolution, &k, testCost, &currentCost);
            // printf("k: %d\nCurrent cost: %.0f\n", k, currentCost);
        } while ( k < num_trucks );

        printf("\n\nCurrent = %.3f || Best = %.3f\n\n", currentCost, bestCost);
        // exit(0);
        
        if( currentCost >= bestCost )
            stop = 1;
        
    } while( !stop );
    
    printf("uhuhuhu\n\n\n\n");
exit(0);
    // return bestSolution;
    // for(int i = 0; i < num_trucks; i++){
    //     graph_set_route(g, i, bestSolution[i], size[i], demandVertices[i]);
    // }
}
// */

/*
        AltereVizinhanças: testar

        Neighborhood_Change_Pipe( S, S", k ){
            se Custo(S") < Custo(S)
                S <- S"
            else
                k++
        }
        
        Neighborhood_Change_Cyclic( S, S", k ){
            k++
            se Custo(S") < Custo(S)
                S <- S"
        }
*/
int **neighborhood_Change_Cyclic(int **currentSolution, int **testSolution, int *k, float testCost, float *currentCost){
    *(k) += 1;
    // printf("\nK: %d -> %d\n", *(k) - 1, *k);
    printf("\nCurrent: %.3f || Test: %.3f\n", *(currentCost), testCost);
    if( testCost < *(currentCost) ){
        // free(currentSolution);
        *(currentCost) = testCost;
        return testSolution;
    }
    // exit(0);
    // free(testSolution);
    return currentSolution;
}
/*
    variable_Neighborhood_Search (VNS):
    S <- S0
    noImp = 2
    while( noImp < 2 ){
        k <- 1
        while ( k < num_trucks ){ 
            S' <- Perturbacao
            S" <- VND
            AltereVizinhanças(S, S", k)
        }
    }
    return s;
*/