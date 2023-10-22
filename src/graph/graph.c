#include "graph.h"
#include "../adjacency_list/list.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"
#include "../algorithms/algorithms.h"

struct Route{
    int size;
    void *route;
    float cost;
    float demand;
};

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

    if( MATRIX ){
        g->adj = matrix_construct(g->num_vertex);

    } else if ( LIST ){
        g->adj = list_construct(g->num_vertex);

    } else {
        exit(printf("Choose a representation. Change the defines in 'graph.h' file\n"));
    }

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

void *route_return_route(Graph *g, int i){
    return (g) ? g->route[i].route : NULL;
}

int route_return_size(Graph *g, int i){
    return (g) ? g->route[i].size : -1;
}

int route_return_demand(Graph *g, int i){
    return (g) ? g->route[i].demand : -1;
}

float route_return_cost(Graph *g, int i){
    return (g) ? g->route[i].cost : -1;
}

float route_return_total_cost(Graph *g){
    if(!g) return -1;
    float cost = 0;
    for(int i = 0; i < g->trucks; i++)
        cost += g->route[i].cost;
    return cost;
}

void graph_add_edge(Graph *g, int v1, int v2, weight peso){
    if( !(v1 - v2) ) return;
    // Se for não direcionado, o menor aponta para o maior
    if( g->direction == UNDIRECTED ) if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }

    if( MATRIX ){
        if( matrix_add_edge(g->adj, v1, v2, peso) == 0) return; 

    } else if ( LIST ){
        if( list_add_edge(g->adj, v1,v2, peso) == 0) return;

    }
    g->num_edge += ( g->direction == UNDIRECTED ) ? 2 : 1;

}

void graph_remove_edge(Graph *g, int v1, int v2){
    if( !(v1 - v2) ) return;
    if( g->direction == UNDIRECTED ) if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }
    ( MATRIX ) ? matrix_remove_edge(g->adj, v1, v2) : list_remove_edge(g->adj, v1, v2);
    g->num_edge--;
}

bool graph_edge_exists(Graph *g, int v1, int v2){
    return ( MATRIX ) ? matrix_edge_exists(g->adj, v1, v2) : list_edge_exists(g->adj, v1, v2);
}

void _trataString(char *string){
    int tam = strlen(string);
    for(int i = 0; i < tam; i++){
        if(string[i] == ' ')
            string[i] = 0;
    }
}

void _read_EUC_2D(Graph *g, FILE *arq){

    int dimension = g->num_vertex;
    float m[dimension][3];

    for(int i = 0; i < dimension; i++)
        fscanf(arq, " %*d %f %f%*c", &m[i][0], &m[i][1]);

    fscanf(arq, "%*[^\n]\n"); // DEMAND_COORD_SECTION

    for(int i = 0; i < dimension; i++){
        
        fscanf(arq, "%*c %f %*c", &m[i][2]);
        int x1 = m[i][0], y1 = m[i][1];
        Data *d = data_construct(x1, y1, m[i][2]);
        vector_push_back(g->vertices, d);

        for(int j = i - 1; j >= 0; j--){
            int x2 = m[j][0], y2 = m[j][1];

            weight w = (weight)sqrt( ( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) ) );

            graph_add_edge(g, i, j, w);
        }
    }

}

void _read_LOWER_ROW(Graph *g, FILE *arq){
    int dimension = g->num_vertex;
    float demand;
    weight weight;

    // Le EDGE_WEIGHT_SECTION
    for(int i = 0; i < dimension; i++){
        for(int j = i + 1; j < dimension; j++){
            fscanf(arq, "%f", &weight);
            graph_add_edge(g, i, j, weight);
        }
    }

    fscanf(arq, "%*[^\n]\n"); // DEMAND_COORD_SECTION

    for(int i = 0; i < dimension; i++){
        fscanf(arq, "%*c %f %*c", &demand);
        Data *d = data_construct(i, i, demand);
        vector_push_back(g->vertices, d);
    }

}

Graph *graph_read_file_CVRPLIB(char *fileName){

    if( !fileName ) exit(printf("ERROR\nrun ./<exe> <CVRP file>\n"));
    FILE *arq = fopen(fileName, "r");
    if( !arq ) exit(printf("ERRO: Falha ao abrir %s\n", fileName));

    char nameprev[64], commentprev[256];
    int dimension = 0, capacity = 0;

    fscanf(arq, "%*[^:]: %s%*c", nameprev); // NAME
    char *name = malloc(sizeof(char) * strlen(nameprev));
    memcpy(name, nameprev, sizeof(char));

    fscanf(arq, "%*[^:]: "); // COMMENT
    fgets(commentprev, 256, arq);
    char *comment = malloc(sizeof(char) * strlen(commentprev));
    memcpy(comment, commentprev, sizeof(char));

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

            scanf("%f", &weight);
            graph_add_edge(g2, i, j, weight);
        }
    }
    scanf("\nDEMAND\n");
    float demand;
    vector_push_back(g2->vertices, data_construct(0,0,0));
    for(int i = 1; i < vx; i++){
        scanf("%f", &demand);
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

    if( MATRIX ){
        matrix_print(g->adj, g->num_vertex);

    } else if ( LIST ){
        list_print(g->adj, g->num_vertex);

    }

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
    
    if( MATRIX ){
        matrix_return_edges(graph_return_adjacencies(g), g->num_vertex, k, graph_return_direction(g));

    } else if( LIST ){
        list_return_edges(graph_return_adjacencies(g), g->num_vertex, sizeEdges, k);

    }

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
        clarke_wright_paralel_algorithm(g, e, near_0, sizeEdges);
    else
        clarke_wright_serial_algorithm(g, e, near_0, sizeEdges);

    free(e);
    free(near_0);
}

void graph_Clarke_Wright_paralel_route(Graph *g){
    graph_Clarke_Wright_route(g, 1);
}

void graph_Clarke_Wright_serial_route(Graph *g){
    graph_Clarke_Wright_route(g, 0);
}

void graph_set_route(Graph *g, int idx, void *route, int size, float demand){
    if( !idx )
        g->route = malloc(sizeof(Route) * g->trucks);
    g->route[idx].route = malloc(sizeof(int) * size);
    g->route[idx].route = memcpy(g->route[idx].route, route, sizeof(int) * size);
    g->route[idx].size = size;
    g->route[idx].demand = demand;
    g->route[idx].cost = matrix_return_route_cost(g->adj, route, size);

    return;
    printf("Rota %d Demanda %.2f Custo %.2f\n", idx, demand, g->route[idx].cost);
    for(int i = 0; i < size; i++){
        int *v = g->route[idx].route;
        printf("%d ", (v[i]));
    }
    printf("\n");
}

void graph_dfs(Graph *g){

    int *route = calloc(g->num_vertex, sizeof(int)), 
        *visited = calloc(g->num_vertex, sizeof(int));

    dfs_algorithm(g->adj, route, visited, g->num_vertex);

    free(visited);
}

void graph_destroy(Graph *g){

    if( MATRIX ){
        matrix_destroy(g->adj, g->num_vertex);

    } else if ( LIST ){
        list_destroy(g->adj, g->num_vertex);

    }

    for(int i = 0; vector_size(g->vertices); i++){
        Data *d = vector_pop_back(g->vertices);
        data_destroy(d);
    }
    vector_destroy(g->vertices);

    for(int i = 0; i < g->trucks; i++)
        free(g->route[i].route);

    free(g->name);
    free(g->comment);
    free(g->route);
    free(g);
}