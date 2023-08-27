#include "graph.h"

struct Graph{
    int num_vertices; 
    int num_edge;
    void *adj;
    Vector *vertices;
};

Graph *graph_construct(int v){
    Graph *g = malloc(sizeof(Graph));

    g->num_edge = 0;
    g->num_vertices = v;
    g->vertices = vector_construct();

    if( MATRIX ){
        g->adj = matrix_construct(g->num_vertices);

    } else if ( LIST ){
        g->adj = list_construct(g->num_vertices);

    } else {
        exit(printf("Choose a representation. Change the defines in 'graph.h' file\n"));
    }

    return g;
}

void graph_add_edge(Graph *g, int v1, int v2, weight peso, int direction){

    if( MATRIX ){
        matrix_add_edge(g->adj, v1, v2, peso, direction);

    } else if ( LIST ){
        list_add_edge(g->adj, v1,v2, peso, direction);

    }
    g->num_edge++;
}

Graph *graph_read_file(){

    char nameprev[500];
    int dimension = 0, capacity = 0;

    scanf("%*[^:]: %s%*c", nameprev);
    char name[ strlen(nameprev) ];
    strcpy(name, nameprev);

    scanf("%*[^\n]\n");
    scanf("%*[^\n]\n");
    scanf("%*[^:]: %d%*c", &dimension);

    scanf("%*[^\n]\n");
    scanf("%*[^:]: %d%*c", &capacity);
    scanf("%*[^\n]\n");

   
    printf("\n%s\n", name);
    printf("%d\n", dimension);
    printf("%d\n", capacity);

    Graph *g = graph_construct(dimension);

    int x = 0, y = 0;
    for(int i = 0; i < dimension; i++){
        scanf(" %*d %d %d%*c", &x, &y);
        Data *d = data_construct(x, y);
        vector_push_back(g->vertices, d);
    }
    scanf("%*[^\n]\n");

    float demand = 0;
    for(int i = 0; i < dimension; i++){
        scanf("%*c %f %*c", &demand);
        Data *d = vector_get(g->vertices, i);
        data_set_demand(d, demand);
    }

    for(int i = 0; i < g->num_vertices ; i++){
        Data *d1 = vector_get(g->vertices, i);
        int x1 = data_return_x(d1), y1 = data_return_y(d1);

        for(int j = 0; j < g->num_vertices; j++){
            Data *d2 = vector_get(g->vertices, j);
            int x2 = data_return_x(d2), y2 = data_return_y(d2);

            weight w = (float)sqrt( ( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) ) );

            graph_add_edge(g, i, j, w, DIRECTED);

        }
    }

    return g;
}

void graph_print(Graph *g){
    printf("Vértices: %d\nArestas: %d\n", g->num_vertices, g->num_edge);

    if( MATRIX ){
        matrix_print(g->adj, g->num_vertices);

    } else if ( LIST ){
        list_print(g->adj, g->num_vertices);

    }

    printf("\nVertices:\n");
    for(int i = 0; i < vector_size(g->vertices); i++){
        Data *d = vector_get(g->vertices, i);
        printf("%d : ", i);
        data_print(d);
    }
}

void graph_destroy(Graph *g){

    if( MATRIX ){
        matrix_destroy(g->adj, g->num_vertices);

    } else if ( LIST ){
        list_destroy(g->adj, g->num_vertices);

    }

    for(int i = 0; vector_size(g->vertices); i++){
        Data *d = vector_pop_back(g->vertices);
        data_destroy(d);
    }
    vector_destroy(g->vertices);

    free(g);
}