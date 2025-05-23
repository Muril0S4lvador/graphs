#include "graph.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"
#include "../algorithms/algorithms.h"
#include "../info/info.h"

struct Graph{
    char *name;
    char *comment;
    int num_vertex; 
    int num_edge;
    int capacity;
    int trucks;
    int optimal;
    int total_demand;
    bool direction;
    void *adj;
    Vector *vertices;
    Route *route;
};

/* =============================================== FUNÇÕES INTERNAS ================================================================== */

// Seleciona versão do algoritmo de Economias
void _graph_Clarke_Wright_route(Graph *g, char control){

    if( g->direction == DIRECTED ){
        printf("Grafo não compatível com o que o algoritmo espera.\n");
        return;
    }

    int sizeEdges = (g->num_edge / 2) - (g->num_vertex - 1);

    Edges *e = calloc( sizeEdges, sizeof(Edges) ), *near_0 = malloc( g->num_vertex * sizeof(Edges) );
    
    matrix_return_edges_savings(graph_return_adjacencies(g), g->num_vertex, e, near_0);

    if(control == 1)
        clarke_wright_parallel_algorithm(g, e, near_0, sizeEdges);
    else
        clarke_wright_serial_algorithm(g, e, near_0, sizeEdges);
    
    info_set_cost_constructive(route_return_total_cost(graph_return_route(g), graph_return_trucks(g)));

    free(e);
    free(near_0);
}

// Retona ótimo da instância a partir de COMMENT
int _return_optimal(char *string){

    int value = 0;
    char *token = strtok(string, " ");
    
    while( token != NULL){
        token[0] = tolower(token[0]);

        if( !strcmp(token, "value:") ){
            token = strtok(NULL, " ");
            sscanf(token, "%d", &value);
        }

        token = strtok(NULL, " ");
    }

    return value;
}

// Lê as arestas de uma instância pela forma EUC_2D (euclidiana)
void _read_EUC_2D(Graph *g, FILE *arq){

    int dimension = g->num_vertex;
    double m[dimension][3];
    char buffer[100];
    
    for(int i = 0; i < dimension; i++){
        fgets(buffer, sizeof(buffer), arq);
        sscanf(buffer, "%*d %lf %lf", &m[i][0], &m[i][1]);
    }

    fgets(buffer, sizeof(buffer), arq); // DEMAND_COORD_SECTION

    for(int i = 0; i < dimension; i++){
        
        fgets(buffer, sizeof(buffer), arq); 
        sscanf(buffer, "%*d %lf", &m[i][2]);

        float x1 = m[i][0], y1 = m[i][1];
        Data *d = data_construct(x1, y1, m[i][2]);
        vector_push_back(g->vertices, d);

        for(int j = i - 1; j >= 0; j--){
            float x2 = m[j][0], y2 = m[j][1];

            weight w = (weight)sqrt( ( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) ) );

            w = round(w);

            graph_add_edge(g, i, j, w);
        }
    }
}

// Lê as arestas de instâncias mundo real
void _read_LOWER_ROW_VIX(Graph *g, FILE *arq){
    int dimension = g->num_vertex;
    int id;

    double m[dimension][3];
    char buffer[100];
    
    for(int i = 0; i < dimension; i++){
        fgets(buffer, sizeof(buffer), arq);
        sscanf(buffer, "%d %lf %lf", &id ,&m[i][0], &m[i][1]);
    }

    fgets(buffer, sizeof(buffer), arq); // DEMAND_COORD_SECTION

    for(int i = 0; i < dimension; i++){
    
        fgets(buffer, sizeof(buffer), arq); 
        sscanf(buffer, "%*d %lf", &m[i][2]);


        float x1 = m[i][0], y1 = m[i][1];
        Data *d = data_construct(x1, y1, m[i][2]);
        data_set_id(d, id);
        vector_push_back(g->vertices, d);

        // Adiciona aresta para deposito em (0, 0)
        weight w = (weight)sqrt( ( pow( (x1 - 0), 2) + pow( (y1 - 0), 2) ) );
        w = round(w);
        graph_add_edge(g, i, 0, w);
    }

    fgets(buffer, sizeof(buffer), arq); // DEMAND_COORD_SECTION

    weight weight;
    for (int i = 0; i < dimension; i++) {
        for (int j = 1; j < dimension; j++) {
            // Verifica se o elemento está no triângulo inferior
            if (i > j) {
                fscanf(arq, "%lf", &weight);
                graph_add_edge(g, j, i, round(weight));
            }
        }
    }
}

// Lê as arestas de uma instância pela forma EXPLICIT LOWER_ROW (Triângulo inferior de uma matriz)
void _read_LOWER_ROW(Graph *g, FILE *arq){
    int dimension = g->num_vertex;
    int demand;
    weight weight;

    // Le EDGE_WEIGHT_SECTION
    for(int i = 0; i < dimension-1; i++){
        for(int j = i + 1; j < dimension; j++){
            fscanf(arq, "%lf", &weight);
            graph_add_edge(g, i, j, weight);
        }
    }

    fscanf(arq, "%*c%*[^\n]\n"); // DEMAND_COORD_SECTION

    for(int i = 0; i < dimension; i++){
        fscanf(arq, "%*c %d %*c", &demand);
        Data *d = data_construct(i, i, demand);
        vector_push_back(g->vertices, d);
    }
}

/* =================================================================================================================================== */

Graph *graph_construct(int v, bool direction){
    Graph *g = malloc(sizeof(Graph));

    g->name = NULL;
    g->comment = NULL;
    g->num_edge = 0;
    g->num_vertex = v;
    g->vertices = vector_construct();
    g->direction = direction;
    g->route = NULL;
    g->capacity = 1;
    g->trucks = 0;
    g->optimal = 0;

    g->adj = matrix_construct(g->num_vertex);

    return g;
}

int graph_return_num_vertex(Graph *g){
    return (g) ? g->num_vertex : -1;
}

int graph_return_num_edges(Graph *g){
    return (g) ? g->num_edge : -1;
}

int graph_return_capacity(Graph *g){
    return (g) ? g->capacity : -1;
}

int graph_return_trucks(Graph *g){
    return (g) ? g->trucks : -1;
}

bool graph_return_direction(Graph *g){
    return g->direction;
}

bool graph_has_route(Graph *g){
    return (g) ? g->route != NULL : 0;
}

void *graph_return_vertex_vector(Graph *g){
    return (g) ? g->vertices : NULL;
}

void *graph_return_adjacencies(Graph *g){
    return (g) ? g->adj : NULL;
}

Route* graph_return_route(Graph *g){
    return (g->route) ? g->route : NULL;
}

int graph_return_optimal_cost(Graph *g){
    return (g) ? g->optimal : -1;
}

char *graph_return_name(Graph *g){
    return (g) ? g->name : "";
}

int graph_return_total_demand(Graph *g){
    return (g) ? g->total_demand : -1;
}

void graph_add_edge(Graph *g, int v1, int v2, weight peso){
    if( !(v1 - v2) ) return;
    // Se for não direcionado, o menor aponta para o maior
    if( g->direction == UNDIRECTED ) if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }

    if( matrix_add_edge(g->adj, v1, v2, peso) == 0 ) return; 

    g->num_edge += ( g->direction == UNDIRECTED ) ? 2 : 1;
}

void graph_remove_edge(Graph *g, int v1, int v2){
    if( !(v1 - v2) ) return;
    if( g->direction == UNDIRECTED ) if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }
    matrix_remove_edge(g->adj, v1, v2);
    g->num_edge--;
}

bool graph_edge_exists(Graph *g, int v1, int v2){
    return matrix_edge_exists(g->adj, v1, v2);
}

Graph *graph_read_file_CVRPLIB(char *fileName){

    if( !fileName ) exit(printf("ERRO\nrun ./<exe> <CVRP file> <X>\n"));
    FILE *arq = fopen(fileName, "r");
    if( !arq ) exit(printf("ERRO: Falha ao abrir %s\n", fileName));

    char buffer[1000];
    int dimension = 0, capacity = 0;

    fgets(buffer, sizeof(buffer), arq); // NAME
    char name[50];
    sscanf(buffer, "%*[^:]: %s", name);

    fgets(buffer, sizeof(buffer), arq); // COMMENT
    char *token = strtok(buffer, "()"); token = strtok(NULL, ")");
    token[strlen(token) - 1] = (token[strlen(token) - 1] == '\n') ? '\0' : token[strlen(token) - 1]; // tira \n caso precise
    char *comment = malloc( sizeof(char) * (strlen(token) + 1) );
    strcpy(comment, token);

    fgets(buffer, sizeof(buffer), arq); // TYPE

    fgets(buffer, sizeof(buffer), arq); // DIMENSION
    sscanf(buffer, "%*[^:]: %d", &dimension);

    char edgeWType[20];
    fgets(buffer, sizeof(buffer), arq); // EDGE_WEIGHT_TYPE
    token = strtok(buffer, ":"); token = strtok(NULL, " ");
    token[strlen(token) - 1] = (token[strlen(token) - 1] == '\n') ? '\0' : token[strlen(token) - 1]; // tira \n caso precise
    strcpy(edgeWType, token);

    if( !strcmp(token, "EXPLICIT") ){
        fgets(buffer, sizeof(buffer), arq); // EDGE_WEIGHT_TYPE
        sscanf(buffer, "%*[^:]: %s", edgeWType);
        fgets(buffer, sizeof(buffer), arq); // DISPLAY_DATA_TYPE
    }
    
    fgets(buffer, sizeof(buffer), arq); // CAPACITY
    sscanf(buffer, "%*[^:]: %d", &capacity);
    
    fgets(buffer, sizeof(buffer), arq); // NODE_COORD_SECTION ou EDGE_WEIGHT_SECTION

    Graph *g = graph_construct(dimension, UNDIRECTED);
    sscanf(name, "%*[^k]k%d" , &g->trucks);
    g->capacity = capacity;
    g->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(g->name, name);
    g->comment = comment;
    g->optimal = _return_optimal(g->comment);

    if( !strcmp(edgeWType, "EUC_2D") ){
        _read_EUC_2D(g, arq);
    } else if( !strcmp(edgeWType, "LOWER_ROW") ){
        _read_LOWER_ROW(g, arq);
    } else if( !strcmp(edgeWType, "LOWER_ROW_VIX") ){
        _read_LOWER_ROW_VIX(g, arq);
    }
    fclose(arq);

    int sum = 0;
    for(int i = 0; i < g->num_vertex; i++){
        sum += data_return_demand(vector_get(g->vertices, i));
    }
    g->total_demand = sum;

    return g;
}

Graph *graph_read_file(){
    int vx, trucks, cap;
    scanf("%d %d %d", &vx, &trucks, &cap);
    Graph *g2 = graph_construct(vx, UNDIRECTED);
    g2->capacity = cap;
    g2->trucks = trucks;
    g2->optimal = 0;
    weight weight;
    scanf("\nDISTANCES\n");
    for(int i = 0; i < vx; i++){
        for(int j = i + 1; j < vx; j++){

            scanf("%lf", &weight);
            graph_add_edge(g2, i, j, weight);
        }
    }
    scanf("\nDEMAND\n");
    int demand;
    vector_push_back(g2->vertices, data_construct(0,0,0));
    for(int i = 1; i < vx; i++){
        scanf("%d", &demand);
        Data *d = data_construct(i, i, demand);
        vector_push_back(g2->vertices, d);
    }

    graph_print(g2);
    return g2;
    
    int v, e;

    scanf("%d %d", &v, &e);

    Graph *g = graph_construct(v, DIRECTED);

    for(int i = 0; i < e; i++){
        int v1 = -1, v2 = -1;
        scanf("%d %d", &v1, &v2);
        graph_add_edge(g, v1, v2, WEIGHT_DEFAULT);
    }
    return g;
}

void graph_print(Graph *g){

    FILE *arq = fopen("Matriz.txt", "w");
    if(!arq){ printf("ERRO: Problema ao imprimir matriz.\n"); return; }

    fprintf(arq, "%s Matrix\n", graph_return_name(g));
    fprintf(arq, "Vértices: %d\nArestas: %d\n", g->num_vertex, g->num_edge);

    matrix_print(g->adj, g->num_vertex, arq);

    fprintf(arq, "\nVertices:\n");
    for(int i = 0; i < vector_size(g->vertices); i++){
        Data *d = vector_get(g->vertices, i);
        fprintf(arq, "%d : ", i);
        data_print(d, arq);
    }

    fclose(arq);
}

Graph *graph_mst_kruskal(Graph *g){

    int sizeEdges = g->num_edge;
    if (g->direction == UNDIRECTED) sizeEdges /= 2;
    
    Edges *k = calloc( g->num_edge, sizeof(Edges) );
    
    matrix_return_edges(graph_return_adjacencies(g), g->num_vertex, k, graph_return_direction(g));

    Graph *mst = kruskal_algorithm(k, g->num_vertex, g->num_edge, g);

    if(!vector_size(g->vertices)) return mst;

    for(int i = 0; i < g->num_vertex; i++){
        Data *d = vector_get(g->vertices, i);
        Data *d_mst = data_construct(data_return_x(d), data_return_y(d), data_return_demand(d));
        vector_push_back(mst->vertices, d_mst);    
    }

    return mst;
}

void graph_Clarke_Wright_parallel_route(Graph *g){
    _graph_Clarke_Wright_route(g, 1);
}

void graph_Clarke_Wright_serial_route(Graph *g){
    _graph_Clarke_Wright_route(g, 0);
}

void graph_set_route(Graph *g, int idx, void *route, int size, int demand){
    if( !idx ) g->route = route_construct(g->trucks);

    route_set_route(g->route, route, idx, size);
    route_set_cost(g->route, matrix_return_route_cost(g->adj, route, size), idx);
    route_set_size(g->route, size, idx);
    route_set_demand(g->route, demand, idx);
}

int *graph_return_demands(Graph *g){
    int *demands = malloc(sizeof(int) * g->num_vertex);
    for(int i = 0; i < g->num_vertex; i++){
        Data *d = vector_get(g->vertices, i);
        demands[i] = data_return_demand(d);
    }
    return demands;
}

void graph_print_routes(Graph *g){
    if (!graph_has_route(g)) return;
    printf("\nCapacity %d\n", graph_return_capacity(g));
    route_print(graph_return_route(g), graph_return_trucks(g));
    printf("\n");
}

int graph_check_routes(char *filename, Graph *g){

    if(!g){printf("ERROR: Need Graph to check the routes\n"); return -1;}

    FILE *f = fopen(filename, "r");
    if(!f){printf("ERROR: Problem with %s\n", filename); return -1; }

    int vertices = 0, num_trucks = 0;
    sscanf(filename, "[^n]n%d-k%d", &vertices, &num_trucks);
    if( vertices != graph_return_num_vertex(g) || num_trucks != graph_return_trucks(g)){
        printf("ERROR: Graph and solution doesn't belong to the same instance.\n"); return -1;
    }

    int num_routes = graph_return_trucks(g), idx = 0;

    int **routes = malloc(sizeof(int*) * num_routes),
        *route_size = calloc(num_routes, sizeof(int)),
        *demands = graph_return_demands(g),
        *route_cost = calloc(num_routes, sizeof(int)),
        *route_demand = calloc(num_routes, sizeof(int));
    
    char linha[500]; // Buffer para armazenar uma linha do arquivo

    while (fgets(linha, sizeof(linha), f) != NULL) {
        
        // Verificar se a linha começa com "Route #"
        if (strncmp(linha, "Route #", 7) == 0) {
            // Se sim, ler os números da rota

            int num, size = 10;
            sscanf(linha, "Route #%*d: ");
            routes[idx] = malloc(sizeof(int) * size);
            routes[idx][route_size[idx]++] = 0;

            char *token = strtok(linha, ":");
            token = strtok(NULL, " ");

            while( token != NULL ){
                sscanf(token, "%d", &num);
                if ( num != 0 ) {
                    routes[idx][route_size[idx]++] = num; 
                    route_demand[idx] += demands[num];
                }

                if( route_size[idx] >= size ){
                    size *= 2;
                    routes[idx] = realloc(routes[idx], sizeof(int) * size);
                }
                token = strtok(NULL, " ");
            }
            routes[idx][route_size[idx]++] = 0;
            idx++;
        }
    }
    fclose(f);

    int num_vertex = graph_return_num_vertex(g);
    int *vtx = calloc(num_vertex, sizeof(int));
    int cost = 0;
    for(int i = 0; i < num_routes; i++){
        route_cost[i] = round( matrix_return_route_cost(graph_return_adjacencies(g), routes[i], route_size[i]) );
        cost += route_cost[i];

        if(route_demand[i] > graph_return_capacity(g)){printf("ERRO demada #%d\n%d | C: %d\n", i+1, route_demand[i], graph_return_capacity(g));}
        
        printf("\nRota %d | [C: %d D: %d]\n", i+1, route_cost[i], route_demand[i]);
        for(int j = 0; j < route_size[i]; j++){
            printf("%d[%d] ", routes[i][j], demands[routes[i][j]]);
            vtx[routes[i][j]]++;
        }
    }
    printf("\nTotal cost: %d\n", cost);

    for(int i = 1; i < num_vertex; i++){
        if(vtx[i] == 0){
            printf("\nVertice %d não aparece na solução.\n", i);
        } else if( vtx[i] > 1 ){
            printf("\nVertice %d aparece na solução %d vezes.\n", i,vtx[i]);
        }
    }

    for(int i = 0; i < num_routes; i++)
        free(routes[i]);
    free(routes);
    free(route_size);
    free(route_demand);
    free(route_cost);
    free(demands);
    free(vtx);

    return cost;
}

void graph_Variable_Neighborhood_Search(Graph *g){
    
    if( !graph_has_route(g) ){
        printf("Necessário a construção de uma solução inicial.\n");
        return;
    }
    
    int **routes = malloc(sizeof(int*) * g->trucks),
        *sizeR = malloc(sizeof(int) * g->trucks),
        *demandsR = malloc(sizeof(int) * g->trucks);
    for(int i = 0; i < g->trucks; i++){
        Route *r = graph_return_route(g);
        routes[i] = route_return_route(r, i);
        sizeR[i] = route_return_size(r, i);
        demandsR[i] = route_return_demand(r, i);
    }
    
    variable_Neighborhood_Search(g, routes, sizeR, graph_return_demands(g), demandsR);

    info_set_cost_vns(route_return_total_cost(graph_return_route(g), graph_return_trucks(g)));    

    free(routes);
    free(sizeR);
    free(demandsR);
}

void graph_2opt(Graph *g){
    double *cost = malloc(sizeof(double) * graph_return_trucks(g));
    Route *r = graph_return_route(g);
    for(int i = 0; i < g->trucks; i++)
        cost[i] = route_return_cost(r, i);

    for(int i = 0; i < g->trucks; i++){
        int *route = route_return_route(r, i),
            size = route_return_size(r, i);
        opt2_algorithm(route, size, g->adj, &cost[i]);
        route_set_cost(g->route, cost[i], i);
    }
    free(cost);
}

void graph_enables_routes(Graph *g){

    if( !graph_has_route(g) ){
        printf("Necessário a construção de uma solução inicial.\n");
        return;
    }
    
    int **routes = malloc(sizeof(int*) * g->trucks),
        *sizeR = malloc(sizeof(int) * g->trucks),
        *demandsR = malloc(sizeof(int) * g->trucks),
        *demands = graph_return_demands(g);
    double *cost = malloc(sizeof(double) * g->trucks);
    Route *r = graph_return_route(g);
    for(int i = 0; i < g->trucks; i++){
        routes[i] = route_return_route(r, i);
        sizeR[i] = route_return_size(r, i);
        demandsR[i] = route_return_demand(r, i);
        cost[i] = route_return_cost(r, i);
    }
    
    enables_route_swap(routes, g->trucks, sizeR, demands, demandsR, g->capacity, cost, g);
    enables_route_reallocate(routes, g->trucks, sizeR, demands, demandsR, g->capacity, cost, g);

    info_set_cost_enables(route_return_total_cost(graph_return_route(g), graph_return_trucks(g)));

    free(routes);
    free(sizeR);
    free(demandsR);
    free(cost);
    free(demands);
}

void graph_route_destroy(Graph *g){
    if ( !graph_has_route(g) ) return;
    Route *r = graph_return_route(g);
    route_destroy(r, g->trucks);
    g->route = NULL;
}

void graph_destroy(Graph *g){

    matrix_destroy(g->adj, g->num_vertex);

    for(int i = 0; vector_size(g->vertices); i++){
        Data *d = vector_pop_back(g->vertices);
        data_destroy(d);
    }
    graph_route_destroy(g);
    vector_destroy(g->vertices);
    free(g->name);
    free(g->comment);
    free(g);
}

void graph_cross_exchange(Graph *g){

    if( !graph_has_route(g) ){
        printf("Necessário a construção de uma solução inicial.\n");
        return;
    }
    
    int **routes = malloc(sizeof(int*) * g->trucks),
        *sizeR = malloc(sizeof(int) * g->trucks),
        *demandsR = malloc(sizeof(int) * g->trucks),
        *demands = graph_return_demands(g),
        size = graph_return_trucks(g);

    double *cost = malloc(sizeof(double) * g->trucks);
    Route *r = graph_return_route(g);
    for(int i = 0; i < g->trucks; i++){
        routes[i] = route_return_route(r, i);
        sizeR[i] = route_return_size(r, i);
        demandsR[i] = route_return_demand(r, i);
        cost[i] = route_return_cost(r, i);
    }

    int r1, v_r1, r2, v_r2;

    r1 = rand() % size;

    v_r1 = rand() % (sizeR[r1] - 3) + 1; //num entre 1 e penultimo

    r2 = r1;
    while( r2 == r1 ){
        r2 = rand() % size;
    }

    v_r2 = rand() % (sizeR[r2] - 3) + 1;

    printf("Capacity: %d", graph_return_capacity(g));
    // printsd(routes, size, sizeR, demandsR, cost, NULL);
    printf("\n\nSize: %d v_r1 = %d\n", sizeR[r1], v_r1);

    void *m = graph_return_adjacencies(g);

    for(int i = v_r1; i < sizeR[r1]; i++){

        if( i >= sizeR[r1] - 2 ) i = i % (sizeR[r1] - 2) + 1;

        int v11 = routes[r1][i], v12 = routes[r1][i + 1];

        int demand1 = demands[v11] + demands[v12];
        int cost1 = matrix_return_edge_weight(m, v11, v12, graph_return_direction(g));

        for(int j = v_r2; j < sizeR[r2]; j++){
            if( j >= sizeR[r2] - 2 ) j = j % (sizeR[r2] - 2) + 1;

            int v21 = routes[r2][j], v22 = routes[r2][j + 1];
            int demand2 = demands[v21] + demands[v22];
            int cost2 = matrix_return_edge_weight(m, v21, v22, graph_return_direction(g));

            int result1 = demandsR[r1] - demand1 + demand2;
            int result2 = demandsR[r2] + demand1 - demand2;
            if( result1 <= graph_return_capacity(g) && result2 <= graph_return_capacity(g) ){
                routes[r1][i] = v21;
                routes[r1][i + 1] = v22;
                routes[r2][j] = v11;
                routes[r2][j + 1] = v12;
                demandsR[r1] = result1;
                demandsR[r2] = result2;
                cost[r1] += cost2 - cost1 + matrix_return_edge_weight(m, v21, routes[r1][i-1], graph_return_direction(g)) + matrix_return_edge_weight(m, v22, routes[r1][i+2], graph_return_direction(g)) - 
                            (matrix_return_edge_weight(m, v11, routes[r1][i-1], graph_return_direction(g)) + matrix_return_edge_weight(m, v12, routes[r1][i+2], graph_return_direction(g)));
                cost[r2] += cost1 - cost2 + matrix_return_edge_weight(m, v11, routes[r2][j-1], graph_return_direction(g)) + matrix_return_edge_weight(m, v12, routes[r2][j+2], graph_return_direction(g)) - 
                            (matrix_return_edge_weight(m, v21, routes[r2][j-1], graph_return_direction(g)) + matrix_return_edge_weight(m, v22, routes[r2][j+2], graph_return_direction(g)));
                i = sizeR[r1];
                j = sizeR[r2];

                printf("%0.lf %0.lf\n", cost[r1], cost[r2]);
                printf("%0.lf %0.lf\n", matrix_return_route_cost(m, routes[r1], sizeR[r1]), matrix_return_route_cost(m, routes[r2], sizeR[r2]));
            }
        }
    }
    // printsd(routes, size, sizeR, demandsR, cost, NULL);

    free(routes);
    free(sizeR);
    free(demandsR);
    free(cost);
    free(demands);
}
