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

void _swap_idx_inRoute(int *idx_inRoute, int idx1, int idx2){
    idx_inRoute[idx2] = idx_inRoute[idx1] ^ idx_inRoute[idx2];
    idx_inRoute[idx1] = idx_inRoute[idx1] ^ idx_inRoute[idx2];
    idx_inRoute[idx2] = idx_inRoute[idx1] ^ idx_inRoute[idx2];
}

void _reverse_route(int lo, int hi, int *route){
    while( hi > lo ){
        _swap_idx_inRoute(route, hi, lo);
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

char _route_delete_vertex(int *route, int *size, int v) {

    int i = 0;
    char control = 0;
    for( i; i < (*size - 1); i++ ){
        if( route[i] == v ){
            control = 1;
            break;
        }
    }
    if(control)
    {
        (*size)--;
        for( i; i < (*size); i++){
            route[i] = route[i+1];
        }
        return 1;
    }
    return 0;
}

void _route_add_vertex(int *route, int *size, int v) {

    for(int i = (*size); i > 1; i--){
        route[i] = route[i - 1];
    }
    route[0] = 0;  // Adiciona o elemento 0
    route[1] = v;  // Adiciona o novo elemento no índice 1

    (*size)++;
}

int **_copy_route_matrix(int **dest, int **src, int size, int *sizeRoute, int num_vertex){
    char fre;
    int **result = malloc(sizeof(int*) * size);
    for(int i = 0; i < size; i++)
    {
        result[i] = malloc(sizeof(int) * num_vertex + 1);
        memcpy(result[i], src[i], sizeof(int) * sizeRoute[i]);
    }
    return result;
}

float _return_total_cost_route(int **routes, int *sizeRoute, int num_trucks, void *graph_adj){
    float sum =0;
    for(int i = 0; i < num_trucks; i++){
        sum += matrix_return_route_cost(graph_adj, routes[i], sizeRoute[i]);
    }
    return sum;
}

void _random_Pertubation(int **routes, int size, int *sizeRoutes, float *demands, int *demandRoutes, int *idx_InRoute){
    srand(time(NULL));

    for(int i = 0; i < size; i++){
        // Gera numero aleatorio entre o 2º e penultimo elemento da rota
        char control = 0;
        while( sizeRoutes[i] <= 2 )
        {
            i++;
            if( i >= size ){
                control = 1;
                break;
            }
        }
        if( control ) break;
        
        int vertex = rand() % (sizeRoutes[i] - 2) + 1, 
            random_route = i;
        
        while( random_route == i )
            random_route = rand() % size;
        
        vertex = routes[i][vertex];
        if( _route_delete_vertex(routes[i], &sizeRoutes[i], vertex) )
        {
        idx_InRoute[vertex] = random_route;
        demandRoutes[i] -= demands[vertex];
        demandRoutes[random_route] += demands[vertex];

        _route_add_vertex(routes[random_route], &sizeRoutes[random_route], vertex);
        }
    }
}

void _realocate_Operator(int **routes, int size, int *sizeRoutes, float *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity){

    for(int i = 1; i < sizeRoutes[k] - 1; i++)
    {
        int vertex = routes[k][i];
        for(int j = 0; j < size; j++)
        {
            if( j == k ) j++;
            if( j >= size ) break;
        
            if( demandRoutes[j]  + demands[vertex] <= capacity )
            {
                if( _route_delete_vertex(routes[k], &sizeRoutes[k], vertex) )
                {
                demandRoutes[j] += demands[vertex];
                demandRoutes[k] -= demands[vertex];
                idx_InRoute[vertex] = j;
                
                _route_add_vertex(routes[j], &sizeRoutes[j], vertex);

                }
            }
        }
    }
}

float _calculate_New_Cost(float currentCost, int v2, void *graph_adj, int *route, int idx_v1){
    float newCost = currentCost;
    int v1 = route[idx_v1];
    newCost -= (matrix_return_edge_weight(graph_adj, v1, route[idx_v1 - 1], UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, v1, route[idx_v1 + 1], UNDIRECTED));
    newCost += (matrix_return_edge_weight(graph_adj, v2, route[idx_v1 - 1], UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, v2, route[idx_v1 + 1], UNDIRECTED));
    return newCost;
}

char _checkDemand(int routeDemand, int v1, int v2, float *demands, int capacity){
    return ( (routeDemand - demands[v1] + demands[v2]) <= capacity ) ? 1 : 0;
}

void _swap_Operator(int **routes, int size, int *sizeRoutes, float *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity, float *cost){
    float BestCost = cost[k];
    int route_Bv2, idx_Bestv1, idx_Bv2;
    char control = 0;

// Swap Operator:
    // Para cada vértice vK de uma rota k:
        // Para cada vértice v' que não está na rota:
            // IF se a demanda da rota K aceita um swap(vK, v'):
                // calcula custo da rota houvesse swap:
                // IF custoNovo < custoAtual:
                    // Salva solução, e parte pra próxima combinação

    // Faz swap(vk, v') no melhor vk e v' achados

// Percorre a rota e, para cada elemento da rota, verifica
// se substituí-lo melhorará no custo da rota, se sim, pegamos ele

    
    // Para cada elemento da rota k
    for(int i = 1; i < sizeRoutes[k] - 1; i++)
    {
        int vertex = routes[k][i];
        for(int j = 0; j < size; j++)
        {
            // Para cada elemento de uma rota j
            for(int l = 1; l < sizeRoutes[j]; l++)
            {
                // Se j e k forem a mesma rota, ignorar
                if( !(j - k) ) continue;
                // Confere se swap(v1, v2) não fere restrição de demanda
                if( !_checkDemand(demandRoutes[k], vertex, routes[j][l], demands, capacity) ) continue;

                // Calcula novo custo da rota K
                float newCostK = _calculate_New_Cost(cost[j], routes[j][l], graph_return_adjacencies(g), routes[k], i);

                // Se for o melhor achado, salva os vertces v1 e v2
                if( newCostK < BestCost ){
                    BestCost = newCostK;
                    idx_Bestv1 = i;
                    idx_Bv2 = l;
                    route_Bv2 = j;
                    control = 1;
                }
            }
        }
    }

    // Se foi encontrado um melhor vértice para um swap, então faça
    if( control )
    {
        demandRoutes[route_Bv2] += ( demands[routes[k][idx_Bestv1]] - demands[routes[route_Bv2][idx_Bv2]] );
        demandRoutes[k]         += ( demands[routes[route_Bv2][idx_Bv2]] - demands[routes[k][idx_Bestv1]] );
        int v1 = routes[k][idx_Bestv1], v2 = routes[route_Bv2][idx_Bv2];
        routes[k][idx_Bestv1] = v2;
        routes[route_Bv2][idx_Bv2] = v1;
    }
}

void _melhorarRotas(int **routes, int size, int *sizeRoutes, void *graph_adj){
    for(int i = 0; i < size; i++)
        opt2_algorithm(routes[i], sizeRoutes[i], graph_adj);
}

char _checkCapacity(int *demands, int size, int capacity){
    for(int i = 0; i < size; i++)
        if( demands[i] > capacity )
            return 0;
    return 1;
}

void printsd(int **routes, int size, int *sizeR, int *demandR){
    for(int i = 0; i < size; i++){
        printf("\nRoute %d (%d): ", i, demandR[i]);
        for(int j = 0; j < sizeR[i]; j++)
        {
            printf("%d ", routes[i][j]);
        }
    }
    printf("\n");
}

void _destroyRoutesMatrix(int **routes, int size){
    for(int i = 0; i < size; i++)
        free(routes[i]);
    free(routes);
}

void variable_Neighborhood_Search(Graph *g, int **routes, int *sizeRoutes, float *demands, int *demandRoutes){

    int num_trucks = graph_return_trucks(g), num_vertex = graph_return_num_vertex(g),
        noImp = 0, k,
        **bestSolution = _copy_route_matrix(NULL, routes, num_trucks, sizeRoutes, num_vertex), // Salva rota inicial
        *best_sizeRoutes =   malloc(sizeof(int) * num_trucks),
        *best_demandRoutes = malloc(sizeof(int) * num_trucks),
        *idx_InRoutes =      malloc(sizeof(int) * num_vertex);


    memcpy(best_sizeRoutes, sizeRoutes, sizeof(int) * num_trucks);      // Salva tamanhos da rota inicial
    memcpy(best_demandRoutes, demandRoutes, sizeof(int) * num_trucks);  // Salva demandas da rota inicial

    for(int i = 0; i < num_trucks; i++)
        for(int j = 1; j < sizeRoutes[i] - 1; j++)
            idx_InRoutes[routes[i][j]] = i;


    float currentCost = _return_total_cost_route(routes, sizeRoutes, num_trucks, graph_return_adjacencies(g)), newCost = 0;
    char possible = _checkCapacity(best_demandRoutes, num_trucks, graph_return_capacity(g));

    printf("\nSolução Inicial(%.3f):\n", currentCost);
    printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes);

    while( noImp < 20000000 )
    {
        k = 0;
        while( k < num_trucks ){

            int *test_sizeR       = malloc(sizeof(int) * num_trucks),
                *test_demandR     = malloc(sizeof(int) * num_trucks),
                *test_idxInroutes = malloc(sizeof(int) * num_vertex);

            memcpy(test_sizeR,       best_sizeRoutes,   sizeof(int) * num_trucks);  // Copia tamanhos da melhor rota atual para modificação
            memcpy(test_demandR,     best_demandRoutes, sizeof(int) * num_trucks);  // Copia demandas da melhor rota atual para modificação
            memcpy(test_idxInroutes, idx_InRoutes,      sizeof(int) * num_vertex);  // Copia vetor idx_InRoute para modificação

            int **solutionTest = _copy_route_matrix(NULL, bestSolution, num_trucks, test_sizeR, num_vertex);    // Copia melhor solução para fazer mudanças

            // _random_Pertubation(solutionTest, num_trucks, test_sizeR, demands, test_demandR, test_idxInroutes); // Gera vizinhança aleatoria

            _realocate_Operator(solutionTest, num_trucks, test_sizeR, demands, test_demandR, test_idxInroutes, k, graph_return_capacity(g)); // Move itens nas rotas

            _melhorarRotas(solutionTest, num_trucks, test_sizeR, graph_return_adjacencies(g)); // Melhora custo intra-rotas
            
            newCost = _return_total_cost_route(solutionTest, test_sizeR, num_trucks, graph_return_adjacencies(g));

                // printf("\nTest encontrado(%.3f | %.3f):\n", newCost, currentCost);
                // printsd(solutionTest, num_trucks, test_sizeR, test_demandR);

            if( newCost < currentCost && _checkCapacity(test_demandR, num_trucks, graph_return_capacity(g)) || 
                ( !possible && _checkCapacity(test_demandR, num_trucks, graph_return_capacity(g)))               ){
                k = 0;
                noImp = 0;
                possible = 1;

                _destroyRoutesMatrix(bestSolution, num_trucks);
                bestSolution = _copy_route_matrix(bestSolution, solutionTest, num_trucks, test_sizeR, num_vertex);
                currentCost = newCost;

                free(best_sizeRoutes);
                free(best_demandRoutes);
                free(idx_InRoutes);

                idx_InRoutes      = malloc(sizeof(int) * num_vertex);
                best_sizeRoutes   = malloc(sizeof(int) * num_trucks);
                best_demandRoutes = malloc(sizeof(int) * num_trucks);

                memcpy(best_sizeRoutes,   test_sizeR,       sizeof(int) * num_trucks);
                memcpy(best_demandRoutes, test_demandR,     sizeof(int) * num_trucks);
                memcpy(idx_InRoutes,      test_idxInroutes, sizeof(int) * num_vertex);   // Copia vetor idx_InRoute para modificação
                
                printf("\nMelhor encontrado(%.3f):\n", newCost);
                printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes);

            } else {
                noImp++;
                k++;
            }
            
            _destroyRoutesMatrix(solutionTest, num_trucks);
            free(test_sizeR);
            free(test_demandR);
            free(test_idxInroutes);
        }
    }

    graph_route_destroy(g);
    for(int i = 0; i < num_trucks; i++){
        graph_set_route(g, i, bestSolution[i], best_sizeRoutes[i], best_demandRoutes[i]);
    }

    free(best_sizeRoutes);
    free(best_demandRoutes);
    free(idx_InRoutes);
    free(demands);
    _destroyRoutesMatrix(bestSolution, num_trucks);
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