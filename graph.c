#include "graph.h"

struct Graph{
    int num_vertices; 
    int num_edge;
    void *adj;
};

Graph *graph_construct(int v){
    Graph *g = malloc(sizeof(Graph));

    g->num_edge = 0;
    g->num_vertices = v;

    if( MATRIX ){
        g->adj = matrix_construct(g->num_vertices);

    } else if ( LIST ){
        g->adj = list_construct(g->num_vertices);

    } else {
        exit(printf("Choose a representation. Change the defines in 'graph.h' file\n"));
    }

    return g;
}

void graph_add_edge(Graph *g, int v1, int v2, height peso, int direction){

    if( MATRIX ){
        matrix_add_edge(g->adj, v1, v2, peso, direction);

    } else if ( LIST ){
        list_add_edge(g->adj, v1,v2, peso, direction);

    }
    g->num_edge++;
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

    if( MATRIX ){
        matrix_print(g->adj, g->num_vertices);

    } else if ( LIST ){
        list_print(g->adj, g->num_vertices);

    }
}

void graph_destroy(Graph *g){

    if( MATRIX ){
        matrix_destroy(g->adj, g->num_vertices);

    } else if ( LIST ){
        list_destroy(g->adj, g->num_vertices);

    }
    free(g);
}