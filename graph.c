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

    Graph *g = graph_construct(dimension);

    float m[dimension][3];

    for(int i = 0; i < dimension; i++)
        scanf(" %*d %f %f%*c", &m[i][0], &m[i][1]);

    scanf("%*[^\n]\n"); // DEMAND_COORD_SECTION

    for(int i = 0; i < g->num_vertices; i++){
        
        scanf("%*c %f %*c", &m[i][2]);
        int x1 = m[i][0], y1 = m[i][1];
        Data *d = data_construct((int)x1, (int)y1, m[i][2]);
        vector_push_back(g->vertices, d);

        for(int j = i - 1; j >= 0; j--){
            int x2 = m[j][0], y2 = m[j][1];

            weight w = (float)sqrt( ( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) ) );

            graph_add_edge(g, i, j, w, DIRECTED);
        }
    }

    return g;
}

Graph *graph_read_file(){
    int v, e;

    scanf("%d %d", &v, &e);

    Graph *g = graph_construct(v);

    for(int i = 0; i < e; i++){
        int v1 = -1, v2 = -1;
        scanf("%d %d", &v1, &v2);
        graph_add_edge(g, v1, v2, WEIGHT_DEFAULT, UNDIRECTED);
    }
    return g;
}

void graph_img_print(Graph *g){

    FILE *arq_vertex = fopen("imgs/vertex.dot", "w");
    char asp = '"';
    fprintf(arq_vertex, "graph {\n");
    fprintf(arq_vertex, "node[fontcolor = white, fillcolor = black, style = filled, shape = circle, width=%c2.5%c, height=%c2.5%c, fontsize = %c50%c];\n"
    , asp, asp, asp, asp, asp, asp);

    for(int i = 0; i < g->num_vertices; i++){
        Data *d = vector_get(g->vertices, i);
        fprintf(arq_vertex, "v%d [pos = %c%d, %d!%c];\n", i, asp, data_return_x(d), data_return_y(d), asp);
    }

    fclose(arq_vertex);
    system("cp imgs/vertex.dot imgs/graph.dot");
    arq_vertex = fopen("imgs/vertex.dot", "a");
    fprintf(arq_vertex, "}");
    fclose(arq_vertex);

    FILE *arq_graph = fopen("imgs/graph.dot", "a");

    for(int i = 0; i < g->num_vertices ; i++)
        for(int j = 0; j < g->num_vertices; j++)
            if( i < j ) fprintf(arq_graph, "v%d -- v%d;\n", i, j);
    
    fprintf(arq_graph, "}");
    fclose(arq_graph);

    system("dot -Kneato -Tpng imgs/vertex.dot -O");
    system("dot -Kneato -Tpng imgs/graph.dot -O");
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