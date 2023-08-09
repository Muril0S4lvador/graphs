#include "graph.h"

Graph *graph_construct(int v){
    Graph *g = malloc(sizeof(Graph));

    g->num_edge = 0;
    g->num_vertices = v;
    g->vertices = malloc(g->num_vertices * sizeof(Vertices));

    for(int i = 0; i < v; i++)
        g->vertices[i].data = g->vertices[i].head = NULL;

    return g;
}

Adjacency *graph_adjacency_create(int vertice, height_type height){
    Adjacency *adj = malloc(sizeof(Adjacency));

    adj->vertice = vertice;
    adj->height = height;
    adj->next = NULL;

    return adj;
}

void graph_add_edge(Graph *g, int vertex, Adjacency *adj, int direction){
    adj->next = g->vertices[vertex].head;
    g->vertices[vertex].head = adj;

    if(direction == UNDIRECTED){
        Adjacency *adj_directed = adjacency_create(vertex, adj->height);
        graph_add_edge(g, adj->vertice, adj_directed, DIRECTED);
        return;
    }

    g->num_edge++;
}

void graph_print(Graph *g){
    printf("Vertices: %d Arestas: %d\n", g->num_vertices, g->num_edge);

    for(int i = 0; i < g->num_vertices; i++){
        printf("v%d: ", i);

        Adjacency *adj = g->vertices[i].head;
        while(adj){
            printf("v%d(%d) ", adj->vertice, adj->height);
            adj = adj->next;
        }
        
        printf("\n");
    }
}

void graph_destroy(Graph *g){
    for(int i = 0; i < g->num_vertices; i++){

        Adjacency *adj = g->vertices[i].head, *aux;

        /* Destruir data, no caso nao tem */
        // free(g->vertices[i].data);

        while(adj){
            aux = adj;
            adj = adj->next;
            free(aux);
        }
    }
    free(g->vertices);
    free(g);
}