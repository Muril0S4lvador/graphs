#include "graph.h"

struct Graph{
    int num_vertices; 
    int num_edge;
    bool direction;
    void *adj;
    Vector *vertices;
};

Graph *graph_construct(int v, bool direction){
    Graph *g = malloc(sizeof(Graph));

    g->num_edge = 0;
    g->num_vertices = v;
    g->vertices = vector_construct();
    g->direction = direction;

    if( MATRIX ){
        g->adj = matrix_construct(g->num_vertices);

    } else if ( LIST ){
        g->adj = list_construct(g->num_vertices);

    } else {
        exit(printf("Choose a representation. Change the defines in 'graph.h' file\n"));
    }

    return g;
}

void graph_add_edge(Graph *g, int v1, int v2, weight peso){
    if( v1 == v2 ) return;
    // Se for não direcionado, o menor aponta para o maior
    if( g->direction == UNDIRECTED ) if( v2 < v1 ) { int aux = v1; v1 = v2; v2 = aux; }

    if( MATRIX ){
        matrix_add_edge(g->adj, v1, v2, peso, g->direction);

    } else if ( LIST ){
        list_add_edge(g->adj, v1,v2, peso, g->direction);

    }
    g->num_edge++;
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

    for(int i = 0; i < g->num_vertices; i++){
        
        scanf("%*c %f %*c", &m[i][2]);
        int x1 = m[i][0], y1 = m[i][1];
        Data *d = data_construct((int)x1, (int)y1, m[i][2]);
        vector_push_back(g->vertices, d);

        for(int j = g->num_vertices - 1; j >= 0; j--){
            int x2 = m[j][0], y2 = m[j][1];

            weight w = (float)sqrt( ( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) ) );

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

void graph_img_print_vertex(Graph *g, char *file_name){

    FILE *arq_vertex = fopen(file_name, "w");
    char asp = '"';

    fprintf(arq_vertex, "graph {\n");
    fprintf(arq_vertex, "node[fontcolor = white, fillcolor = black, style = filled, shape = circle, fontsize = %c12.5%c];\n", asp, asp);

    for(int i = 0; i < g->num_vertices; i++){
        Data *d = vector_get(g->vertices, i);
        fprintf(arq_vertex, "v%d [pos = %c%.2f, %.2f!%c];\n", i, asp, (float)data_return_x(d)/3, (float)data_return_y(d)/3, asp);
    }

    fprintf(arq_vertex, "}");
    fclose(arq_vertex);

    system("dot -Kneato -Tpng imgs/vertex.dot -O &");
}

void graph_print(Graph *g){
    printf("Vértices: %d\nArestas: %d\n", g->num_vertices, g->num_edge);

    if( MATRIX ){
        matrix_print(g->adj, g->num_vertices);

    } else if ( LIST ){
        list_print(g->adj, g->num_vertices);

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