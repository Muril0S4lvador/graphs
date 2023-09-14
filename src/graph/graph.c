#include "graph.h"

struct Graph{
    int num_vertex; 
    int num_edge;
    bool direction;
    void *adj;
    Vector *vertices;
};

Graph *graph_construct(int v, bool direction){
    Graph *g = malloc(sizeof(Graph));

    g->num_edge = 0;
    g->num_vertex = v;
    g->vertices = vector_construct();
    g->direction = direction;

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

bool graph_return_direction(Graph *g){
    return g->direction;
}

Vector *graph_return_vertex_vector(Graph *g){
    return (g) ? g->vertices : NULL;
}

void *graph_return_adjacencies(Graph *g){
    return (g) ? g->adj : NULL;
}

void graph_add_edge(Graph *g, int v1, int v2, weight peso){
    if( v1 == v2 ) return;
    // Se for não direcionado, o menor aponta para o maior
    if( g->direction == UNDIRECTED ) if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }

    if( MATRIX ){
        matrix_add_edge(g->adj, v1, v2, peso);

    } else if ( LIST ){
        list_add_edge(g->adj, v1,v2, peso);

    }
    g->num_edge += ( g->direction == UNDIRECTED ) ? 2 : 1;

}

void graph_remove_edge(Graph *g, int v1, int v2){
    ( MATRIX ) ? matrix_remove_edge(g->adj, v1, v2) : list_remove_edge(g->adj, v1, v2);
}

bool graph_edge_exists(Graph *g, int v1, int v2){
    return ( MATRIX ) ? matrix_edge_exists(g->adj, v1, v2) : list_edge_exists(g->adj, v1, v2);
}

Graph *graph_read_file_CVRPLIB(){

    char nameprev[500];
    int dimension = 0, capacity = 0;

    scanf("%*[^:]: %s%*c", nameprev);
    char name[ strlen(nameprev) ];
    strcpy(name, nameprev);

    scanf("%*[^\n]\n"); // COMMENT
    scanf("%*[^\n]\n"); // TYPE
    scanf("%*[^:]: %d%*c", &dimension);

    scanf("%*[^\n]\n"); // EDGE_WEIGHT_TYPE
    scanf("%*[^:]: %d%*c", &capacity);
    scanf("%*[^\n]\n"); //NODE_COORD_SECTION

    printf("\n%s\n", name);
    printf("%d\n", dimension);
    printf("%d\n", capacity);

    Graph *g = graph_construct(dimension, UNDIRECTED);

    float m[dimension][3];

    for(int i = 0; i < dimension; i++)
        scanf(" %*d %f %f%*c", &m[i][0], &m[i][1]);

    scanf("%*[^\n]\n"); // DEMAND_COORD_SECTION

    for(int i = 0; i < g->num_vertex; i++){
        
        scanf("%*c %f %*c", &m[i][2]);
        int x1 = m[i][0], y1 = m[i][1];
        Data *d = data_construct((int)x1, (int)y1, m[i][2]);
        vector_push_back(g->vertices, d);

        for(int j = i - 1; j >= 0; j--){
            int x2 = m[j][0], y2 = m[j][1];

            weight w = (weight)sqrt( ( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) ) );

            graph_add_edge(g, i, j, w);
        }
    }

    return g;
}

Graph *graph_read_file(){
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

Graph *graph_kruskal(Graph *g){

}

void graph_print(Graph *g){
    printf("Vértices: %d\nArestas: %d\n", g->num_vertex, g->num_edge);

    if( MATRIX ){
        matrix_print(g->adj, g->num_vertex);

    } else if ( LIST ){
        list_print(g->adj, g->num_vertex);

    }

/*
    printf("\nVertices:\n");
    for(int i = 0; i < vector_size(g->vertices); i++){
        Data *d = vector_get(g->vertices, i);
        printf("%d : ", i);
        data_print(d);
    }
*/
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

    free(g);
}