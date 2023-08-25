#include "graph.h"

Graph *graph_construct(int v){
    Graph *g = malloc(sizeof(Graph));

    g->num_edges = 0;
    g->num_vertices = v;
    g->adj = malloc(v * sizeof(int*));

    for(int i = 0; i < v; i++)
        g->adj[i] = calloc(v, sizeof(int));

    return g;
}

void graph_add_edge(Graph *g, int v1, int v2, height peso, int direction){
    if(direction == UNDIRECTED)
        g->adj[v2][v1] = peso;

    g->adj[v1][v2] = peso;

    g->num_edges++;
}

Graph *graph_read_file(Graph *g){

    /*
    char nameprev[500000];
    scanf("%*[^:]: ");
    scanf("%s", nameprev);
    char name[ strlen(nameprev) ];
    strcpy(name, nameprev);
    Essa função acima lê corretamente o nome, 
    basta fazer p outras situações que funciona
    */

   

}

void graph_print(Graph *g){
    printf("-- ");
    for(int i = 0; i < g->num_vertices; i++)
        printf("v%d ", i);

    for(int i = 0; i < g->num_vertices; i++){
        printf("\n");
        printf("v%d:", i);

        for(int j = 0; j < g->num_vertices; j++)
            printf(" %d", g->adj[i][j]);

    }
    printf("\n");
}

void graph_destroy(Graph *g){
    for(int i = 0; i < g->num_vertices; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}