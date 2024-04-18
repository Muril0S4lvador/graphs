#include "graph.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"
#include "../algorithms/algorithms.h"

struct Graph{
    char *name;
    char *comment;
    int num_vertex; 
    int num_edge;
    int capacity;
    int trucks;
    bool direction;
    void *adj;
    Vector *vertices;
    Route *route;
};

/* =============================================== FUNÇÕES INTERNAS ================================================================== */

// Trata a string para ser comparável a outra string
void _trataString(char *string){
    int tam = strlen(string);
    for(int i = 0; i < tam; i++){
        if(string[i] == ' ')
            string[i] = 0;
    }
}

// Lê as arestas de uma instância pela forma EUC_2D (euclidiana)
void _read_EUC_2D(Graph *g, FILE *arq){

    int dimension = g->num_vertex;
    double m[dimension][3];

    for(int i = 0; i < dimension; i++)
        fscanf(arq, " %*d %lf %lf%*c", &m[i][0], &m[i][1]);

    fscanf(arq, "%*[^\n]\n"); // DEMAND_COORD_SECTION

    for(int i = 0; i < dimension; i++){
        
        fscanf(arq, "%*c %lf %*c", &m[i][2]);
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

double graph_return_optimal_cost(Graph *g){
    if(!g) return -1;
    double cost = -1;
    char value[7] = "value:", comment[strlen(g->comment) + 1], *token = NULL;
    memcpy(comment, g->comment, strlen(g->comment) + 1);

    token = strtok(comment, " ");
    while( token ){
        if( !strcmp(token, value) ){
            token = strtok(NULL, " ");
            sscanf(token, "%lf", &cost);
        }
        token = strtok(NULL, " ");
    }
    return cost;
}

char *graph_return_name(Graph *g){
    return (g) ? g->name : "";
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

    if( !fileName ) exit(printf("ERROR\nrun ./<exe> <CVRP file>\n"));
    FILE *arq = fopen(fileName, "r");
    if( !arq ) exit(printf("ERRO: Falha ao abrir %s\n", fileName));

    char nameprev[1000], commentprev[256];
    int dimension = 0, capacity = 0;

    fscanf(arq, "%*[^:]: %s%*c", nameprev); // NAME
    char *name = malloc(sizeof(char) * strlen(nameprev) + 2);
    memcpy(name, nameprev, strlen(nameprev) + 1);

    fscanf(arq, "%*[^:]: "); // COMMENT
    fgets(commentprev, 256, arq);
    char *comment = malloc(sizeof(char) * strlen(commentprev) + 1);
    memcpy(comment, commentprev, strlen(commentprev) + 1);

    fscanf(arq, "%*[^\n]\n"); // TYPE

    fscanf(arq, "%*[^:]: %d%*c", &dimension); // DIMENSION

    char edgeWType[20] = "";
    fscanf(arq, "%*[^:]: "); // EDGE_WEIGHT_TYPE
    fscanf(arq, "%[^\n]\n", edgeWType); // TYPE
    _trataString(edgeWType);

    if( !strcmp(edgeWType, "EXPLICIT") ){
        fscanf(arq, "%*[^:]: "); // EDGE_WEIGHT_FORMAT
        fscanf(arq, "%[^\n]\n", edgeWType);
        _trataString(edgeWType);
        fscanf(arq, "%*[^\n]\n"); //DISPLAY_DATA_TYPE
    }
    
    fscanf(arq, "%*[^:]: %d%*c", &capacity); // CAPACITY
    
    fscanf(arq, "%*[^\n]\n"); //NODE_COORD_SECTION ou EDGE_WEIGHT_SECTION

    Graph *g = graph_construct(dimension, UNDIRECTED);
    sscanf(nameprev, "%*[^k]k%d" , &g->trucks);
    g->capacity = capacity;
    g->name = name;
    g->comment = comment;

    if( !strcmp(edgeWType, "EUC_2D") ){
        _read_EUC_2D(g, arq);
    } else if( !strcmp(edgeWType, "LOWER_ROW") ){
        _read_LOWER_ROW(g, arq);
    }
    fclose(arq);

    return g;
}

Graph *graph_read_file(){
    int vx, trucks, cap;
    scanf("%d %d %d", &vx, &trucks, &cap);
    Graph *g2 = graph_construct(vx, UNDIRECTED);
    g2->capacity = cap;
    g2->trucks = trucks;
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
    printf("Vértices: %d\nArestas: %d\n", g->num_vertex, g->num_edge);

    matrix_print(g->adj, g->num_vertex);

    printf("\nVertices:\n");
    for(int i = 0; i < vector_size(g->vertices); i++){
        Data *d = vector_get(g->vertices, i);
        printf("%d : ", i);
        data_print(d);
    }
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

void graph_Clarke_Wright_route(Graph *g, char control){

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

    free(e);
    free(near_0);
}

void graph_Clarke_Wright_parallel_route(Graph *g){
    graph_Clarke_Wright_route(g, 1);
}

void graph_Clarke_Wright_serial_route(Graph *g){
    graph_Clarke_Wright_route(g, 0);
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