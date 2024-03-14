#include "algorithms.h"
#include "../graphviz_print/graphviz_print.h"
#include "../union_find/union_find.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"

/* =============================================== FUNÇÕES INTERNAS ================================================================== */

// Troca os valores dos índices idx1 e idx2
void _swap_idx_inRoute(int *idx_inRoute, int idx1, int idx2){
    idx_inRoute[idx2] = idx_inRoute[idx1] ^ idx_inRoute[idx2];
    idx_inRoute[idx1] = idx_inRoute[idx1] ^ idx_inRoute[idx2];
    idx_inRoute[idx2] = idx_inRoute[idx1] ^ idx_inRoute[idx2];
}

// Reverte a rota no intervalo (lo, hi)
void _reverse_route(int lo, int hi, int *route){
    while( hi > lo ){
        _swap_idx_inRoute(route, hi, lo);
        hi--;
        lo++;
    }
}

// Deleta vértice v de uma rota
char _route_delete_vertex(int *route, int *size, int v, double *cost, void *graph_adj) {
    // Se encontrar vértice v na rota joga pro final e decrementa tamanho

    int i = 0;
    char control = 0;
    for( i; i < (*size - 1); i++ ){
        if( route[i] == v ){
            control = 1;
            double dim = matrix_return_edge_weight(graph_adj, v, route[i - 1], UNDIRECTED) +
                        matrix_return_edge_weight(graph_adj, v, route[i + 1], UNDIRECTED);

            double add = matrix_return_edge_weight(graph_adj, route[i - 1], route[i + 1], UNDIRECTED);

            (*cost) += add - dim;
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

// Adiciona vértice v no início de uma rota
void _route_add_vertex(int *route, int *size, int v, double *cost, void *graph_adj) {
    // Switch dos elementos para direita
    for(int i = (*size); i > 1; i--){
        route[i] = route[i - 1];
    }
    route[0] = 0;
    route[1] = v;  // Adiciona o novo elemento no índice 1
    double add = matrix_return_edge_weight(graph_adj, v, route[0], UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, v, route[2], UNDIRECTED);

    double dim = matrix_return_edge_weight(graph_adj, route[0], route[2], UNDIRECTED);
    (*cost) += add - dim;

    (*size)++;
}

// Copia a matriz src para dest
int **_copy_route_matrix(int **dest, int **src, int size, int *sizeRoute, int num_vertex){
    int **result = malloc(sizeof(int*) * size);
    for(int i = 0; i < size; i++)
    {
        result[i] = malloc(sizeof(int) * num_vertex + 1);
        memcpy(result[i], src[i], sizeof(int) * sizeRoute[i]);
    }
    return result;
}

// Retorna custo total de uma rota
double _return_total_cost_route(int **routes, int *sizeRoute, int num_trucks, void *graph_adj){
    double sum =0;
    for(int i = 0; i < num_trucks; i++){
        sum += matrix_return_route_cost(graph_adj, routes[i], sizeRoute[i]);
    }
    return sum;
}

char _checkVetor(int *vet, int size){
    for(int i = 0; i < size; i++)
        if( vet[i] == 0 ) return 0;
    return 1;
}

void _printfvet(int *vet, int size){
    for(int i = 0; i < size; i++){
        printf("%d", vet[i]);
        if(i < size - 1) printf(", ");
    }
    printf("]\n");
}

void _fillvet(int *vet, int size){
    for(int i = 0; i < size; i++)
        vet[i] = 1;
}

// Tira um elemento de cada rota e coloca em outra aleatória (Não garante validez)
void _random_Pertubation(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity, double *cost, void *graph_adj, Graph *g){

    // printf("\n====================REALLOCATE============================");
    // printf("\nAntes");
    // printsd(routes, size, sizeRoutes, demandRoutes, cost, demands);
    // printf("\n\n");

    int vezes[size];
    for(int i = 0; i < size; i++) vezes[i] = 0;

    srand(time(NULL) + clock());

    k = rand() % size;  //rota aletoria

    char change = 0;

    int route_src[size], route_dest[size];
    for(int i = 0; i < size; i++) route_src[i] = 0;

    int routeOrigem = k;

    while(!_checkVetor(route_src, size)) // Enquanto todas as rotas nao serem testadas como origem
    { 
        // Seleciona a rota aleatoria Garante que nao repetira e sera aleatorio
        while(route_src[routeOrigem]){
            routeOrigem = rand() % size; // Rota aleatoria

            // printf("Route src: %d(%d) - [", routeOrigem, route_src[routeOrigem]);
            // _printfvet(route_src, size);
        }
        route_src[routeOrigem] = 1;

        int vertexRSRC[sizeRoutes[routeOrigem]]; // Vetor controlando quais vértices eu testei da rota aleatoria
        for(int i = 0; i < sizeRoutes[routeOrigem]; i++) vertexRSRC[i] = 0;
        vertexRSRC[0] = 1;
        vertexRSRC[sizeRoutes[routeOrigem] - 1] = 1;

        srand(time(NULL) + clock());  
        while(!_checkVetor(vertexRSRC, sizeRoutes[routeOrigem])) // Enquanto todos os vértices da rota origem nao forem testados
        { 

            int vertex = 0;
            // Garante que nao repetira vertices
            while(vertexRSRC[vertex]){
                vertex = rand() % (sizeRoutes[routeOrigem] - 2) + 1; // Selecionando um vértice aleatório dessa rota

                // printf("Vertex: %d - [", routes[routeOrigem][vertex]);
                // _printfvet(vertexRSRC, sizeRoutes[routeOrigem]);
            }

            vertexRSRC[vertex] = 1;
            vertex = routes[routeOrigem][vertex];

            for(int i = 0; i < size; i++) route_dest[i] = 0;

            while(!_checkVetor(route_dest, size)) // Enquanto todas as rotas nao serem testadas como destino
            { 

                int routeDestino = routeOrigem;
                routeDestino = rand() % size; // Seleciona rota aleatoria, que nao a mesma da anterior
                route_dest[routeOrigem] = 1;
                while(route_dest[routeDestino] || routeDestino == routeOrigem){

                    routeDestino = (routeDestino + 1) % size;

                    // printf("Route dest: %d(%d) - [", routeDestino, route_dest[routeDestino]);
                    // _printfvet(route_dest, size);

                }
                route_dest[routeDestino] = 1;

                // tenta adicionar
                // printf("Testing vertex %d from %d to %d\n", vertex, routeOrigem, routeDestino);
                if( demandRoutes[routeDestino]  + demands[vertex] <= capacity )
                {
                    if( _route_delete_vertex(routes[routeOrigem], &sizeRoutes[routeOrigem], vertex, &cost[routeOrigem], graph_adj) )
                    {
                        demandRoutes[routeDestino] += demands[vertex];
                        demandRoutes[routeOrigem] -= demands[vertex];
                        
                        _route_add_vertex(routes[routeDestino], &sizeRoutes[routeDestino], vertex, &cost[routeDestino], graph_adj);

                        // enables_route_swap(routes, size, sizeRoutes, demands, demandRoutes, capacity, cost, g);
                        // enables_route_reallocate(routes, size, sizeRoutes, demands, demandRoutes, capacity, cost, g);
                        printf("\nPerturbing %d from %d to %d\n", vertex, routeOrigem, routeDestino);


                        _fillvet(route_dest, size);
                        _fillvet(vertexRSRC, sizeRoutes[routeOrigem]);

                        // printf("\nAccepted (%d em %d -> %d)", vertex, routeOrigem, routeDestino);
                        // printsd(routes, size, sizeRoutes, demandRoutes, cost, demands);

                    }
                }
            }
        }
    }

    // printf("\nDepois");
    // printsd(routes, size, sizeRoutes, demandRoutes, cost, demands);

    // Sem mais alterações possíveis
    return;
}

// Retira um vértice de uma rota e coloca em outra que seja válida (não busca melhora)
void _move_Pertubation(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity, double *cost, void *graph_adj, Graph *g){

    // printf("\n====================REALLOCATE============================");
    // printf("\nAntes");
    // printsd(routes, size, sizeRoutes, demandRoutes, cost, demands);
    // printf("\n\n");

    int vezes[size];
    for(int i = 0; i < size; i++) vezes[i] = 0;

    srand(time(NULL) + clock());

    k = rand() % size;  //rota aletoria

    char change = 0;

    int route_src[size], route_dest[size];
    for(int i = 0; i < size; i++) route_src[i] = 0;

    int routeOrigem = k;

    while(!_checkVetor(route_src, size)) // Enquanto todas as rotas nao serem testadas como origem
    { 
        // Seleciona a rota aleatoria Garante que nao repetira e sera aleatorio
        while(route_src[routeOrigem]){
            routeOrigem = rand() % size; // Rota aleatoria

            // printf("Route src: %d(%d) - [", routeOrigem, route_src[routeOrigem]);
            // _printfvet(route_src, size);
        }
        route_src[routeOrigem] = 1;

        int vertexRSRC[sizeRoutes[routeOrigem]]; // Vetor controlando quais vértices eu testei da rota aleatoria
        for(int i = 0; i < sizeRoutes[routeOrigem]; i++) vertexRSRC[i] = 0;
        vertexRSRC[0] = 1;
        vertexRSRC[sizeRoutes[routeOrigem] - 1] = 1;

        srand(time(NULL) + clock());  
        while(!_checkVetor(vertexRSRC, sizeRoutes[routeOrigem])) // Enquanto todos os vértices da rota origem nao forem testados
        { 

            int vertex = 0;
            // Garante que nao repetira vertices
            while(vertexRSRC[vertex]){
                vertex = rand() % (sizeRoutes[routeOrigem] - 2) + 1; // Selecionando um vértice aleatório dessa rota

                // printf("Vertex: %d - [", routes[routeOrigem][vertex]);
                // _printfvet(vertexRSRC, sizeRoutes[routeOrigem]);
            }

            vertexRSRC[vertex] = 1;
            vertex = routes[routeOrigem][vertex];

            for(int i = 0; i < size; i++) route_dest[i] = 0;

            while(!_checkVetor(route_dest, size)) // Enquanto todas as rotas nao serem testadas como destino
            { 

                int routeDestino = routeOrigem;
                routeDestino = rand() % size; // Seleciona rota aleatoria, que nao a mesma da anterior
                route_dest[routeOrigem] = 1;
                while(route_dest[routeDestino] || routeDestino == routeOrigem){

                    routeDestino = (routeDestino + 1) % size;

                    // printf("Route dest: %d(%d) - [", routeDestino, route_dest[routeDestino]);
                    // _printfvet(route_dest, size);

                }
                route_dest[routeDestino] = 1;

                // tenta adicionar
                // printf("Testing vertex %d from %d to %d\n", vertex, routeOrigem, routeDestino);
                if( demandRoutes[routeDestino]  + demands[vertex] <= capacity )
                {
                    if( _route_delete_vertex(routes[routeOrigem], &sizeRoutes[routeOrigem], vertex, &cost[routeOrigem], graph_adj) )
                    {
                        demandRoutes[routeDestino] += demands[vertex];
                        demandRoutes[routeOrigem] -= demands[vertex];
                        
                        _route_add_vertex(routes[routeDestino], &sizeRoutes[routeDestino], vertex, &cost[routeDestino], graph_adj);

                        // enables_route_swap(routes, size, sizeRoutes, demands, demandRoutes, capacity, cost, g);
                        // enables_route_reallocate(routes, size, sizeRoutes, demands, demandRoutes, capacity, cost, g);

                        // opt2_algorithm(routes[routeOrigem], sizeRoutes[routeOrigem], graph_adj, cost); // Ajeita os vértices na melhor posição possível

                        printf("\nMoving %d from %d to %d\n", vertex, routeOrigem, routeDestino);

                        // _fillvet(route_dest, size);
                        // _fillvet(vertexRSRC, sizeRoutes[routeOrigem]);

                        // printf("\nAccepted (%d em %d -> %d)", vertex, routeOrigem, routeDestino);
                        // printsd(routes, size, sizeRoutes, demandRoutes, cost, demands);

                        return;
                    }
                }
            }
        }
    }

    // printf("\nDepois");
    // printsd(routes, size, sizeRoutes, demandRoutes, cost, demands);

    // Sem mais alterações possíveis
    return;
}

void _shake(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity, double *cost, void *graph_adj, Graph *g){

    int div = NUM_IT / 2;
    if( k >= 0 && k < div ){
        _move_Pertubation(routes, size, sizeRoutes, demands, demandRoutes, NULL, k, capacity, cost, graph_return_adjacencies(g), g);
    } else {
        _random_Pertubation(routes, size, sizeRoutes, demands, demandRoutes, NULL, k, capacity, cost, graph_return_adjacencies(g), g);
    }

    printf("\n");
    printsd(routes, size, sizeRoutes, demandRoutes, cost, NULL);
    printf("\n");
}

// Calcula novo custo de rotas trocando dois vértices de lugar
double _calculate_New_Cost(int *route, double currentCost, int new_v, int idx_old_v, int size, void *graph_adj){
    // Retira arestas ((v-1), v) e (v, (v+1)) e adiciona ((v-1, v')) e (v', v+1) no custo da rota
    
    double newCost = currentCost;
    int v1 = route[idx_old_v];
    double DIM = (matrix_return_edge_weight(graph_adj, v1, route[idx_old_v - 1], UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, v1, route[idx_old_v + 1], UNDIRECTED));

    double ADD = (matrix_return_edge_weight(graph_adj, new_v, route[idx_old_v - 1], UNDIRECTED) +
                matrix_return_edge_weight(graph_adj, new_v, route[idx_old_v + 1], UNDIRECTED));
    
    newCost = newCost - DIM + ADD;

    return newCost;
}

// Retorna se a demanda trocando dois vértices é válida ou não
char _checkDemand(int routeDemand, int old_v, int new_v, int *demands, int capacity){
    return ( (routeDemand - demands[old_v] + demands[new_v]) <= capacity ) ? 1 : 0;
}

char _reallocate_Operator(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity, double *cost, void *graph_adj, Graph *g){
    int vezes[size];
    for(int i = 0; i < size; i++) vezes[i] = 0;

    srand(time(NULL) + clock());

    k = rand() % size;  //rota aletoria

    char change = 0;

    int route_src[size], route_dest[size];
    for(int i = 0; i < size; i++) route_src[i] = 0;

    int routeOrigem = k;

    while(!_checkVetor(route_src, size)) // Enquanto todas as rotas nao serem testadas como origem
    { 
        // Seleciona a rota aleatoria Garante que nao repetira e sera aleatorio
        while(route_src[routeOrigem]){
            routeOrigem = rand() % size; // Rota aleatoria

            // printf("Route src: %d(%d) - [", routeOrigem, route_src[routeOrigem]);
            // _printfvet(route_src, size);
        }
        route_src[routeOrigem] = 1;

        int vertexRSRC[sizeRoutes[routeOrigem]]; // Vetor controlando quais vértices eu testei da rota aleatoria
        for(int i = 0; i < sizeRoutes[routeOrigem]; i++) vertexRSRC[i] = 0;
        vertexRSRC[0] = 1;
        vertexRSRC[sizeRoutes[routeOrigem] - 1] = 1;

        srand(time(NULL) + clock());  
        while(!_checkVetor(vertexRSRC, sizeRoutes[routeOrigem])) // Enquanto todos os vértices da rota origem nao forem testados
        { 

            int idx_vertex = 0;
            // Garante que nao repetira vertices
            while(vertexRSRC[idx_vertex]){
                idx_vertex = rand() % (sizeRoutes[routeOrigem] - 2) + 1; // Selecionando um vértice aleatório dessa rota

                // printf("Vertex: %d - [", routes[routeOrigem][idx_vertex]);
                // _printfvet(vertexRSRC, sizeRoutes[routeOrigem]);
            }

            vertexRSRC[idx_vertex] = 1;
            int vertex = routes[routeOrigem][idx_vertex];

            for(int i = 0; i < size; i++) route_dest[i] = 0;

            while(!_checkVetor(route_dest, size)) // Enquanto todas as rotas nao serem testadas como destino
            { 

                int routeDestino = routeOrigem;
                routeDestino = rand() % size; // Seleciona rota aleatoria, que nao a mesma da anterior
                route_dest[routeOrigem] = 1;
                while(route_dest[routeDestino] || routeDestino == routeOrigem){

                    routeDestino = (routeDestino + 1) % size;

                    // printf("Route dest: %d(%d) - [", routeDestino, route_dest[routeDestino]);
                    // _printfvet(route_dest, size);

                }
                route_dest[routeDestino] = 1;

                // tenta adicionar
                // printf("Testing vertex %d from %d to %d\n", vertex, routeOrigem, routeDestino);
                if( demandRoutes[routeDestino]  + demands[vertex] <= capacity )
                {

                    // Salva custo x->i->y
                    double costXIY = matrix_return_edge_weight(graph_adj, vertex, routes[routeOrigem][idx_vertex - 1], graph_return_direction(g))
                                    + matrix_return_edge_weight(graph_adj, vertex, routes[routeOrigem][idx_vertex + 1], graph_return_direction(g));

                    for(int i = 1; i < sizeRoutes[routeDestino] - 1; i++){
                        double costAIB = matrix_return_edge_weight(graph_adj, vertex, routes[routeDestino][i - 1], graph_return_direction(g)) // Custo a->i->b
                                        + matrix_return_edge_weight(graph_adj, vertex, routes[routeDestino][i + 1], graph_return_direction(g)),
                        new_cost = costAIB + matrix_return_edge_weight(graph_adj, routes[routeOrigem][idx_vertex - 1], routes[routeOrigem][idx_vertex + 1], graph_return_direction(g)), // Custo a->i->b + x->y
                        old_cost = costXIY + matrix_return_edge_weight(graph_adj, routes[routeDestino][i - 1], routes[routeDestino][i + 1], graph_return_direction(g)); // Custo x->i->y + a->b

                        // Se custo a->i->y + x->y < a->b + x->y->i
                        if( new_cost < old_cost ){

                            printf("%0.lf < %0.lf\n", new_cost, old_cost);

                            if(_route_delete_vertex(routes[routeOrigem], &sizeRoutes[routeOrigem], vertex, &cost[routeOrigem], graph_adj)){

                                demandRoutes[routeDestino] += demands[vertex];
                                demandRoutes[routeOrigem]  -= demands[vertex];

                                for(int j = sizeRoutes[routeDestino]; j > i; j--)
                                    routes[routeDestino][j] = routes[routeDestino][j - 1];
                                
                                routes[routeDestino][i] = vertex;
                                sizeRoutes[routeDestino]++;
                                cost[routeDestino] += costAIB - matrix_return_edge_weight(graph_adj, routes[routeDestino][i - 1], routes[routeDestino][i + 1], graph_return_direction(g));;
                                // RouteOrigem já tem isso acertado em _route_delete_vertex()

                                printf("Tirando %d em %d e adicionando em %d\n", vertex, routeOrigem, routeDestino);

                                // Se encontrou vizinhança melhor, aceita e sai
                                // Caso não, busca outro destino|vértice|origem (nessa ordem)
                                return 1;
                            }
                        }
                    }

                    // if( _route_delete_vertex(routes[routeOrigem], &sizeRoutes[routeOrigem], vertex, &cost[routeOrigem], graph_adj) )
                    // {
                        // demandRoutes[routeDestino] += demands[vertex];
                        // demandRoutes[routeOrigem] -= demands[vertex];

                        // Ve a melhor posicao para o vertex na rotaDestino e atualiza tamanho e custo
                    // }
                }
            }
        }
    }
    // Retorna sem fazer alterações
    return 0;
}

// Pega a primeira melhoria de troca entre operadores de uma rota k e outra qualquer
char _swap_Operator(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int *idx_InRoute, int k, int capacity, double *cost, Graph *g){

    double bestCost = 0; // Melhor custo (atual)
    for(int i = 0; i < size; i++) bestCost += cost[i];

    int vezes[size];
    for(int i = 0; i < size; i++) vezes[i] = 0;

    srand(time(NULL) + clock());

    k = rand() % size;  //rota aletoria

    char change = 0;

    int route_src[size], route_dest[size];
    for(int i = 0; i < size; i++) route_src[i] = 0;

    int routeOrigem = k;

    while(!_checkVetor(route_src, size)) // Enquanto todas as rotas nao serem testadas como origem
    { 
        // Seleciona a rota aleatoria Garante que nao repetira e sera aleatorio
        while(route_src[routeOrigem]){
            routeOrigem = rand() % size; // Rota aleatoria

            // printf("Route src: %d(%d) - [", routeOrigem, route_src[routeOrigem]);
            // _printfvet(route_src, size);
        }
        route_src[routeOrigem] = 1;

        for(int i = 0; i < size; i++) route_dest[i] = 0;

        while(!_checkVetor(route_dest, size)) // Enquanto todas as rotas nao serem testadas como destino
        { 

            int routeDestino = routeOrigem;
            routeDestino = rand() % size; // Seleciona rota aleatoria, que nao a mesma da anterior
            route_dest[routeOrigem] = 1;
            while(route_dest[routeDestino] || routeDestino == routeOrigem){

                routeDestino = (routeDestino + 1) % size;

                // printf("Route dest: %d(%d) - [", routeDestino, route_dest[routeDestino]);
                // _printfvet(route_dest, size);

            }
            route_dest[routeDestino] = 1;

            for(int i = 1; i < sizeRoutes[routeOrigem] - 1; i++)
            {
                int vertex = routes[routeOrigem][i];

                // Para cada elemento de uma rota j
                for(int l = 1; l < sizeRoutes[routeDestino] - 1; l++)
                {
                    // Se j e routeOrigem forem a mesma rota, ignorar
                    if( !(routeDestino - routeOrigem) ) continue;
                    // Confere se swap(v1, v2) não fere restrição de demanda
                    if( !_checkDemand(demandRoutes[routeOrigem], vertex, routes[routeDestino][l], demands, capacity) 
                        || !_checkDemand(demandRoutes[routeDestino], routes[routeDestino][l], vertex, demands, capacity) ) continue;

                    // Calcula novo custo da rota solução
                    double newCost = _calculate_New_Cost(routes[routeOrigem], cost[routeOrigem], routes[routeDestino][l], i, sizeRoutes[routeOrigem], graph_return_adjacencies(g))
                                    + _calculate_New_Cost(routes[routeDestino], cost[routeDestino], routes[routeOrigem][i], l, sizeRoutes[routeDestino], graph_return_adjacencies(g))
                                    - cost[routeOrigem] - cost[routeDestino];

                    // Se for o primeiro achado, salva os vertces v1 e v2
                    if( newCost < bestCost ){

                        demandRoutes[routeDestino] += ( demands[routes[routeOrigem][i]] - demands[routes[routeDestino][l]] );
                        demandRoutes[routeOrigem] += ( demands[routes[routeDestino][l]] - demands[routes[routeOrigem][i]] );
                        int v1 = routes[routeOrigem][i], v2 = routes[routeDestino][l];
                        cost[routeOrigem] = _calculate_New_Cost(routes[routeOrigem], cost[routeOrigem], v2, i, sizeRoutes[routeOrigem], graph_return_adjacencies(g));
                        cost[routeDestino] = _calculate_New_Cost(routes[routeDestino], cost[routeDestino], v1, l, sizeRoutes[routeDestino], graph_return_adjacencies(g));
                        routes[routeOrigem][i] = v2;
                        routes[routeDestino][l] = v1;
                        return 1;
                    }
                }
            }
           
        }
    }
    // Retorna sem fazer alterações
    return 0;
}

// Aplica algoritmo 2OPT em todas as rotas
void _melhorarRotas(int **routes, int size, int *sizeRoutes, double *cost,void *graph_adj){
    for(int i = 0; i < size; i++)
        opt2_algorithm(routes[i], sizeRoutes[i], graph_adj, &cost[i]);
}

// Retorna se as rotas possuem demandas válidas ou não
char _checkCapacity(int *routeDemands, int size, int capacity){
    for(int i = 0; i < size; i++)
        if( routeDemands[i] > capacity || routeDemands[i] < 1 )
            return 0;
    return 1;
}

// Destrói matrix de rotas
void _destroyRoutesMatrix(int **routes, int size){
    for(int i = 0; i < size; i++)
        free(routes[i]);
    free(routes);
}

/* =================================================================================================================================== */

Graph *kruskal_algorithm(Edges *k, int num_vertex, int num_edges, Graph *g){

    UF* uf = UF_init(num_vertex);
    Graph *mst = graph_construct(num_vertex, UNDIRECTED);
    
    qsort(k, num_edges, sizeof(Edges), edges_compare_growing); // Ordena crescente vetor de arestas

    // Para cada aresta, verifica se estão de alguma forma conectada, caso não, conecta
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

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);                      // Ordena decrescente as economias
    qsort(near_0, graph_return_num_vertex(g)-1, sizeof(Edges), edges_compare_growing); // Ordena crescrente os vértices + próx de 0

    int num_vertex = graph_return_num_vertex(g),            // Número de vértices do grafo
        num_trucks = graph_return_trucks(g),                // Número de rotas/caminhões do grafo
        size_act_route = 0,                                 // Tamanho da rota local
        capacity = graph_return_capacity(g),                // Capacidade do caminhão
        *act_route = malloc( sizeof(int) * num_vertex ),    // Vetor contendo rota atual
        *gl_visited = calloc( num_vertex,  sizeof(int) ),   // Vetor bool de vértices ja inseridos em alguma rota
        *demands = graph_return_demands(g),                 // Vetor de demanda dos vértices
        demand_act_route = 0;                               // Demanda da rota atual

    char control = 0;   // Controle se a rota foi criada ou nao

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

            // Se ja varremos todo o vetor de Edges, mas existe ainda um elemento
            // que consegue encaixar na rota atual, voltamos do início para encontrar ele
            if( j == sizeEdges - 1 ){
                for(int d = 1; d < num_vertex; d++){
                    if( !gl_visited[d] && demand_act_route + demands[d] <= capacity ){
                        j = -1;
                        break;
                    }
                }
            }
        } // Fim for edges e

        // Se control = 1, quer dizer que encontramos uma rota possível
        // Então adicionamos 0 no início e no final para ajudar na impressão depois
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

    qsort(e, sizeEdges, sizeof(Edges), edges_compare_descending);   // Ordena o vetor de economias decrescente

   int  num_vertex = graph_return_num_vertex(g),                // Número de vértices no grafo
        num_trucks = graph_return_trucks(g),                    // Número de caminhões no grafo
        **route = malloc(sizeof(int*) * num_trucks),            // Matriz de rotas
        *visited = calloc( num_vertex, sizeof(int) ),           // Vetor bool se vértice foi visitado ou não
        noVisiteds = num_vertex - 1,                            // Número de vértices não visitados
        *size = malloc(sizeof(int) * num_trucks),               // Tamanho de cada rota
        capacity = graph_return_capacity(g),                   // Capacidade dos caminhões
        *demands = graph_return_demands(g),                    // Vetor de demanda de todos os vertices
        *demand_route = malloc(sizeof(int) * num_trucks);   // vetor de demanda das rotas

    char imp;   // Variável para controle de adição nas rotas

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
                    // Se a rota atual não possui tamaho, pegamos a primeira aresta que não foi visitado e adicionamos na rota
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
                    // Se imp == 0, quer dizer que não foi adicionado nenhum vértice nas rotas, 
                    // mas que ainda faltam vértices a serem adicionados em rotas. Isso quer dizer
                    // que as todas as rotas estão com a demanda cheia.
                    // Para resolver isso, adicionamos todos os vértices adjacentes na última rota
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

    // Adicionamos 0 no início e no final de todas as rotas para facilitar
    // a visualização depois
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

void opt2_algorithm(int *route, int sizeRoute, void *graph_adj, double *cost){
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

                    (*cost) += (new - cur_weight);
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

int **variable_Neighborhood_Descent(int **routes, int *sizeRoutes, int *idx_InRoutes, int *demandRoutes, double *costRoutes, int *demands, Graph *g){

        int num_trucks = graph_return_trucks(g), num_vertex = graph_return_num_vertex(g),      // Número de rotas/caminhões
        capacity = graph_return_capacity(g),                                                   // Capacidade da rota/caminhão
        k = 0,                                                                                 // Rota atual a ser manipulada
        **bestSolution = _copy_route_matrix(NULL, routes, num_trucks, sizeRoutes, num_vertex), // Melhor solução achada
        *best_sizeRoutes =   malloc(sizeof(int) * num_trucks),                                 // Tamanho das rotas na melhor solução achada
        *best_demandRoutes = malloc(sizeof(int) * num_trucks);                                 // Demanda das rotas na melhor solução achada

    double currentCost = _return_total_cost_route(routes, sizeRoutes, num_trucks, graph_return_adjacencies(g)),  // Variável com o custo da solução atual
           newCost = 0,                                                                                          // Variável com o custo da nova solução encontrada
            *best_costRoutes = malloc(sizeof(double) * num_trucks);                                              // Custo das rotas na melhor solução achada


    // Inicializa variáveis
    memcpy(best_sizeRoutes, sizeRoutes, sizeof(int) * num_trucks);      // Salva tamanhos da rota inicial (Melhor solução encontrada)
    memcpy(best_demandRoutes, demandRoutes, sizeof(int) * num_trucks);  // Salva demandas da rota inicial (Melhor solução encontrada)
    memcpy(best_costRoutes, costRoutes, sizeof(double) * num_trucks);   // Salva custo da rota inicial (Melhor solução encontrada)


    int *test_sizeR       = malloc(sizeof(int) * num_trucks),   // Tamanho das rotas da nova solução encontrada
        *test_demandR     = malloc(sizeof(int) * num_trucks);   // Demanda das rotas da nova solução encontrada
    double *test_costR     = malloc(sizeof(double) * num_trucks); // Vetor com custo de todas as rotas da nova solução encontrada

    k = 0;

    while( k < NEIGHBORHOOD_STRUCTURES )
    {

        memcpy(test_sizeR,       best_sizeRoutes,   sizeof(int) * num_trucks);  // Copia tamanhos da melhor rota atual para modificação
        memcpy(test_demandR,     best_demandRoutes, sizeof(int) * num_trucks);  // Copia demandas da melhor rota atual para modificação
        memcpy(test_costR      , best_costRoutes,        sizeof(double) * num_trucks);// Copia vetor costRoutes para modificação

        int **solutionTest; // Matriz de rotas com a melhor solução para manipulação 
        solutionTest = _copy_route_matrix(NULL, bestSolution, num_trucks, test_sizeR, num_vertex);    // Copia melhor solução para fazer mudanças

        switch(k){
            case 0:
            printf("RELLOCATE\n");
                (void)_reallocate_Operator(solutionTest, num_trucks, test_sizeR, demands, test_demandR, NULL, k, capacity, test_costR, graph_return_adjacencies(g), g);
                break;
            case 1:
            printf("SWAP\n");
                (void)_swap_Operator(solutionTest, num_trucks, test_sizeR, demands, test_demandR, NULL, k, capacity, test_costR, g);
                break;
            default:
            printf("OPT\n");
                opt2_inter_routes(solutionTest, num_trucks, test_sizeR, test_demandR, test_costR, demands, g);
                break;
        }

        printf("\n");
        printsd(solutionTest, num_trucks, test_sizeR, test_demandR, test_costR, NULL);
        printf("\n");
        
        newCost = 0;
        for(int i = 0; i < num_trucks; i++){
            newCost += test_costR[i];
        }

        // printf("\n%lf | %lf\n", newCost, currentCost);
        // exit(90);

        if( newCost < currentCost /*&& _checkCapacity(test_demandR, num_trucks, graph_return_capacity(g))*/ ){
            printf("k = 0\n");
            k = 0;

            _destroyRoutesMatrix(bestSolution, num_trucks);
            bestSolution = _copy_route_matrix(bestSolution, solutionTest, num_trucks, test_sizeR, num_vertex);
            currentCost = newCost;

            memcpy(best_sizeRoutes, test_sizeR, sizeof(int) * num_trucks);
            memcpy(best_demandRoutes, test_demandR, sizeof(int) * num_trucks);
            memcpy(best_costRoutes, test_costR, sizeof(double) * num_trucks);

        } else {
            k++;
        }
        _destroyRoutesMatrix(solutionTest, num_trucks);
    }

    memcpy(sizeRoutes, best_sizeRoutes, sizeof(int) * num_trucks);
    memcpy(demandRoutes, best_demandRoutes, sizeof(int) * num_trucks);
    memcpy(costRoutes, best_costRoutes, sizeof(double) * num_trucks);

    _destroyRoutesMatrix(routes, num_trucks);

    free(best_sizeRoutes);
    free(best_demandRoutes);
    free(best_costRoutes);
    free(test_sizeR);
    free(test_demandR);
    free(test_costR);

    return bestSolution;
}

void variable_Neighborhood_Search(Graph *g, int **routes, int *sizeRoutes, int *demands, int *demandRoutes){

    int num_trucks = graph_return_trucks(g), num_vertex = graph_return_num_vertex(g),          // Número de rotas/caminhões
        noImp = 0,                                                                             // Quantidade de vezes que não houve melhora da solução
        k,                                                                                     // Rota atual a ser manipulada
        **bestSolution = _copy_route_matrix(NULL, routes, num_trucks, sizeRoutes, num_vertex), // Melhor solução achada
        *best_sizeRoutes =   malloc(sizeof(int) * num_trucks),                                 // Tamanho das rotas na melhor solução achada
        *best_demandRoutes = malloc(sizeof(int) * num_trucks),                                 // Demanda das rotas na melhor solução achada
        *idx_InRoutes =      malloc(sizeof(int) * num_vertex);                                 // Vetor que salva o índice da rota dos vértices

    double currentCost = _return_total_cost_route(routes, sizeRoutes, num_trucks, graph_return_adjacencies(g)),  // Variável com o custo da solução atual
           newCost = 0,                                                                                          // Variável com o custo da nova solução encontrada
          *costRoutes = malloc(sizeof(double) * num_trucks);                                                     // Vetor com custo de todas as rotas

    // Inicializa variáveis
    memcpy(best_sizeRoutes, sizeRoutes, sizeof(int) * num_trucks);      // Salva tamanhos da rota inicial (Melhor solução encontrada)
    memcpy(best_demandRoutes, demandRoutes, sizeof(int) * num_trucks);  // Salva demandas da rota inicial (Melhor solução encontrada)

    for(int i = 0; i < num_trucks; i++){
        costRoutes[i] = route_return_cost(g, i);
        for(int j = 1; j < sizeRoutes[i] - 1; j++)
            idx_InRoutes[routes[i][j]] = i;
    }

            // printf("\nSolução Inicial(%.3f):\n", currentCost);
            // printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes, costRoutes, demands);
    
    double c1, c2, c3;

    // printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~ INITIAL ~~~~~~~~~~~~~~\n");
    // printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes, costRoutes, demands);

    
    while( noImp < NUM_IT )
    {
        k = 0;
        while( k < NEIGHBORHOOD_STRUCTURES ){

            int *test_sizeR       = malloc(sizeof(int) * num_trucks),   // Tamanho das rotas da nova solução encontrada
                *test_demandR     = malloc(sizeof(int) * num_trucks),   // Demanda das rotas da nova solução encontrada
                *test_idxInroutes = malloc(sizeof(int) * num_vertex);   // Vetor que salva o índice da rota dos vértices da nova solução encontrada
            double *test_costR     = malloc(sizeof(double) * num_trucks); // Vetor com custo de todas as rotas da nova solução encontrada

            memcpy(test_sizeR,       best_sizeRoutes,   sizeof(int) * num_trucks);  // Copia tamanhos da melhor rota atual para modificação
            memcpy(test_demandR,     best_demandRoutes, sizeof(int) * num_trucks);  // Copia demandas da melhor rota atual para modificação
            memcpy(test_idxInroutes, idx_InRoutes,      sizeof(int) * num_vertex);  // Copia vetor idx_InRoute para modificação
            memcpy(test_costR      , costRoutes,        sizeof(double) * num_trucks);// Copia vetor costRoutes para modificação

            int **solutionTest; // Matriz de rotas com a melhor solução para manipulação 
            solutionTest = _copy_route_matrix(NULL, bestSolution, num_trucks, test_sizeR, num_vertex);    // Copia melhor solução para fazer mudanças

            // Transformar a vizinhança que terá a realocação mais aleatória
            _shake(solutionTest, num_trucks, test_sizeR, demands, test_demandR, test_idxInroutes, k, graph_return_capacity(g), test_costR, graph_return_adjacencies(g), g); // Move itens nas rotas

            solutionTest = variable_Neighborhood_Descent(solutionTest, test_sizeR, test_idxInroutes, test_demandR, test_costR, demands, g);

    // printf("\n********** FIND ****************\n");
    // printsd(solutionTest, num_trucks, test_sizeR, test_demandR, test_costR, demands);

            newCost = 0;
            for(int i = 0; i < num_trucks; i++){
                newCost += test_costR[i];
            }

            // Se melhorou o custo, pegamos a solução
            if( newCost < currentCost /*&& _checkCapacity(test_demandR, num_trucks, graph_return_capacity(g))*/ ){
                k = 0;
                noImp = 0;

                _destroyRoutesMatrix(bestSolution, num_trucks);
                bestSolution = _copy_route_matrix(bestSolution, solutionTest, num_trucks, test_sizeR, num_vertex);
                currentCost = newCost;

                free(best_sizeRoutes);
                free(best_demandRoutes);
                free(idx_InRoutes);
                free(costRoutes);

                best_sizeRoutes   = test_sizeR;
                best_demandRoutes = test_demandR;
                idx_InRoutes      = test_idxInroutes;
                costRoutes        = test_costR;

                test_sizeR = NULL;
                test_demandR = NULL;
                test_idxInroutes = NULL;
                test_costR = NULL;

                // printf("\nSAVED\n");

                        // printf("\nMelhor encontrado(%.3f):\n", newCost);
                        // printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes, costRoutes, demandRoutes);

            } else {
                noImp++;
                k++;
            }
            
            _destroyRoutesMatrix(solutionTest, num_trucks);
            free(test_sizeR);
            free(test_demandR);
            free(test_idxInroutes);
            free(test_costR);
        }
    }

    // printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes, costRoutes, NULL);
    printsd(bestSolution, num_trucks, best_sizeRoutes, best_demandRoutes, costRoutes, demands);

    graph_route_destroy(g);
    for(int i = 0; i < num_trucks; i++){
        graph_set_route(g, i, bestSolution[i], best_sizeRoutes[i], best_demandRoutes[i]);
    }

    free(best_sizeRoutes);
    free(best_demandRoutes);
    free(idx_InRoutes);
    free(demands);
    free(costRoutes);
    _destroyRoutesMatrix(bestSolution, num_trucks);
}

void enables_route_swap(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int capacity, double *cost, Graph *g){

    for(int i = 0; i < size; i++){
        
        if( demandRoutes[i] <= capacity ) continue;

        for(int j = 0; j < size; j++){

            if( i == j ) continue;

            int sizeI = sizeRoutes[i] - 1;
            for(int k = 1; k < sizeI; k++){

                int sizeJ = sizeRoutes[j] - 1;
                for(int l = 1; l < sizeJ; l++){

                    // Se trocar vertices da rota k com algum da rota i
                    // e diminuir custo, aceitar
                    int demanda1 = demandRoutes[i] - demands[routes[i][k]] + demands[routes[j][l]],
                        demanda2 = demandRoutes[j] + demands[routes[i][k]] - demands[routes[j][l]];

                    if( demanda1 < demandRoutes[i] && demanda2 <= capacity ){
                        demandRoutes[i] = demanda1;
                        demandRoutes[j] = demanda2;

                        // Calcula novo custo
                        cost[i] = _calculate_New_Cost(routes[i], cost[i], routes[j][l], k, sizeRoutes[i], graph_return_adjacencies(g));
                        cost[j] = _calculate_New_Cost(routes[j], cost[j], routes[i][k], l, sizeRoutes[j], graph_return_adjacencies(g));

                        // Troca Vértices
                        routes[i][k] = routes[i][k] ^ routes[j][l];
                        routes[j][l] = routes[i][k] ^ routes[j][l];
                        routes[i][k] = routes[i][k] ^ routes[j][l];

                        if( demandRoutes[i] <= capacity ){
                            k = sizeI;
                            break;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < size; i++){
        route_set_demand(g, i, demandRoutes[i]);
        route_set_cost(g, i, cost[i]);
    }
}

void enables_route_reallocate(int **routes, int size, int *sizeRoutes, int *demands, int *demandRoutes, int capacity, double *cost, Graph *g){

    int **solution = _copy_route_matrix(solution, routes, size, sizeRoutes, graph_return_num_vertex(g));

    for(int i = 0; i < size; i++)
    {
        
        if( demandRoutes[i] <= capacity ) continue;

        int sizeI = sizeRoutes[i] - 1;
        for(int k = 1; k < sizeI; k++)
        {
            for(int j = 0; j < size; j++)
            {

                if( i == j ) continue;

                // Se trocar vertices da rota k com algum da rota i
                // e diminuir custo, aceitar
                int vertex = solution[i][k],
                    demanda1 = demandRoutes[i] - demands[vertex],
                    demanda2 = demandRoutes[j] + demands[vertex];

                if( demanda1 < demandRoutes[i] && demanda2 <= capacity )
                {
                    demandRoutes[i] = demanda1;
                    demandRoutes[j] = demanda2;

                    // Reposiciona o vértice e calcula novo custo
                    _route_delete_vertex(solution[i], &sizeRoutes[i], vertex, &cost[i], graph_return_adjacencies(g));
                    _route_add_vertex(solution[j], &sizeRoutes[j], vertex, &cost[j], graph_return_adjacencies(g));

                    if( demandRoutes[i] <= capacity ){
                        k = sizeI;
                        break;
                    }
                }
            }
        }
    }

    graph_route_destroy(g);
    for(int i = 0; i < size; i++)
        graph_set_route(g, i, solution[i], sizeRoutes[i], demandRoutes[i]);
    
    _destroyRoutesMatrix(solution, size);
}

/* =============================================== APAGAR DEPOIS ================================================================== */

void printsd(int **routes, int size, int *sizeR, int *demandR, double *cost, int *demands){
    for(int i = 0; i < size; i++){
        printf("\nRoute %d (%d|%.3f): ", i, demandR[i], cost[i]);
        for(int j = 0; j < sizeR[i]; j++)
        {
            printf("%d", routes[i][j]);
            if(demands) printf("[%d] ", demands[routes[i][j]]);
            else printf(" ");
        }
    }
    printf("\n");
}

void printa_rota(int *r, int size, int *demands, int capacity, Graph *g){
    int d = 0;
    for(int i = 0; i < size; i++) d += demands[r[i]];
    double cost = matrix_return_route_cost(graph_return_adjacencies(g), r, size);
    printf("%.3lf | %d/%d: ", cost, d, capacity);
    for(int i = 0; i < size; i++) printf("%d ", r[i]);
    printf("\n");
}

int _return_demand(int *r, int size, int *demands, Graph *g){
    int d = 0;
    for(int i = 0; i < size; i++) d += demands[r[i]];
    return d;
}

void opt2_inter_routes(int **routes, int size, int *sizeR, int *demandR, double *cost, int *demands, Graph *g){

    int num_trucks = graph_return_trucks(g), num_vertex = graph_return_num_vertex(g),          // Número de rotas/caminhões
        noImp = 0,                                                                             // Quantidade de vezes que não houve melhora da solução
        k,                                                                                     // Rota atual a ser manipulada
        **bestSolution = _copy_route_matrix(NULL, routes, num_trucks, sizeR, num_vertex), // Melhor solução achada
        *best_sizeR =   malloc(sizeof(int) * num_trucks),                                 // Tamanho das rotas na melhor solução achada
        *best_demandRoutes = malloc(sizeof(int) * num_trucks),                                 // Demanda das rotas na melhor solução achada
        *idx_InRoutes =      malloc(sizeof(int) * num_vertex);                                 // Vetor que salva o índice da rota dos vértices

    double currentCost = _return_total_cost_route(routes, sizeR, num_trucks, graph_return_adjacencies(g)),  // Variável com o custo da solução atual
           newCost = 0,                                                                                          // Variável com o custo da nova solução encontrada
          *costRoutes = malloc(sizeof(double) * num_trucks);                                                     // Vetor com custo de todas as rotas

    // Inicializa variáveis
    memcpy(best_sizeR, sizeR, sizeof(int) * num_trucks);      // Salva tamanhos da rota inicial (Melhor solução encontrada)
    memcpy(best_demandRoutes, demandR, sizeof(int) * num_trucks);

    int vezes[size];
    for(int i = 0; i < size; i++) vezes[i] = 0;

    srand(time(NULL) + clock());

    char change = 0;

    int route_src[size], route_dest[size];
    for(int i = 0; i < size; i++) route_src[i] = 0;

    int routeOrigem = rand() % size;

    while(!_checkVetor(route_src, size)) // Enquanto todas as rotas nao serem testadas como origem
    { 
        // Seleciona a rota aleatoria Garante que nao repetira e sera aleatorio
        while(route_src[routeOrigem]){
            routeOrigem = rand() % size; // Rota aleatoria
        }
        route_src[routeOrigem] = 1;

        for(int i = 0; i < size; i++) route_dest[i] = 0;

        srand(time(NULL) + clock());
        while(!_checkVetor(route_dest, size)) // Enquanto todas as rotas nao serem testadas como destino
        { 

            int routeDestino = routeOrigem;
            routeDestino = rand() % size; // Seleciona rota aleatoria, que nao a mesma da anterior
            route_dest[routeOrigem] = 1;
            while(route_dest[routeDestino]){
                routeDestino = (routeDestino + 1) % size;
            }
            route_dest[routeDestino] = 1;

            // Nesse ponto, já temos dest e origem aleatoria            
            // Vamos achar as arestas

            for(int i = 0; i < sizeR[routeOrigem] - 2; i++) // Vértices em Origem
            {
                for(int j = 0; j < sizeR[routeDestino] - 2; j++) // Vértices em Destino
                {
                    if( i + j == 0 ) j++;

                    int vi1 = routes[routeOrigem][i],  vi2 = routes[routeOrigem][i+1],
                        vj1 = routes[routeDestino][j], vj2 = routes[routeDestino][(j+1) % (sizeR[routeDestino] - 1)];
                    // Selecionei 2 vértices em cada rota

                    double custo_atual = matrix_return_edge_weight(graph_return_adjacencies(g), vi1, vi2, graph_return_direction(g))
                                       + matrix_return_edge_weight(graph_return_adjacencies(g), vj1, vj2, graph_return_direction(g));
                    
                    double custo_novo  = matrix_return_edge_weight(graph_return_adjacencies(g), vi1, vj1, graph_return_direction(g))
                                       + matrix_return_edge_weight(graph_return_adjacencies(g), vi2, vj2, graph_return_direction(g));
                    
                    if(custo_novo < custo_atual)
                    {
                        int rota1[graph_return_num_vertex(g) + 1],
                            rota2[graph_return_num_vertex(g) + 1];

                        // Escreve rota1 em um vetor
                        int k1 = 0;
                        for(int k = 0; k < (i + 1); k++, k1++){
                            if( k == (i + 1) ) break;
                            rota1[k1] = routes[routeOrigem][k];
                        }
                        for(int k = j; k >= 0; k--, k1++){
                            rota1[k1] = routes[routeDestino][k];
                        }

                        // Escreve rota2 em um vetor
                        int k2 = 0;
                        for(int k = sizeR[routeOrigem] - 1; k > i; k--, k2++){
                            rota2[k2] = routes[routeOrigem][k];
                        }
                        for(int k = j+1; k < sizeR[routeDestino]; k++, k2++){
                            rota2[k2] = routes[routeDestino][k];
                        }

                        // printa_rota(routes[routeOrigem], sizeR[routeOrigem], demands, graph_return_capacity(g), g);
                        // printa_rota(routes[routeDestino], sizeR[routeDestino], demands, graph_return_capacity(g), g);
                        // printa_rota(rota1, k1, demands, graph_return_capacity(g), g);
                        // printa_rota(rota2, k2, demands, graph_return_capacity(g), g);

                        int demand1 = _return_demand(rota1, k1, demands, g),
                            demand2 = _return_demand(rota2, k2, demands, g);

                        int capacity = graph_return_capacity(g);
                        // printf("Naum axei niadja\n");
                        if(demand1 <= capacity && demand2 <= capacity){
                            // printf("Achado uma melhor!\n");
                            
                            memcpy(routes[routeOrigem], rota1, sizeof(int) * k1);
                            memcpy(routes[routeDestino], rota2, sizeof(int) * k2);

                            demandR[routeOrigem] = demand1;
                            demandR[routeDestino] = demand2;

                            sizeR[routeOrigem] = k1;
                            sizeR[routeDestino] = k2;

                            cost[routeOrigem] = matrix_return_route_cost(graph_return_adjacencies(g), routes[routeOrigem], sizeR[routeOrigem]);
                            cost[routeDestino] = matrix_return_route_cost(graph_return_adjacencies(g), routes[routeDestino], sizeR[routeDestino]);

                            return;
                        }
                    }
                }
            }
        }
    }

    // Sem mais alterações possíveis
    return;
}
