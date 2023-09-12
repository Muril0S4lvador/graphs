#include "list.h"

struct Adjacency{
    int vertice;
    weight weight;
    struct Adjacency *next;
};

struct List{
    // data_type *data;
    Adjacency *head;
};

List list_construct(int v){

    List vtx = malloc(v * sizeof(struct List));

    for(int i = 0; i < v; i++)
        // vtx[i].data = 
        vtx[i].head = NULL;

    return vtx;
}

Adjacency *adjacency_create(int vertice, weight weight){
    Adjacency *adj = malloc(sizeof(Adjacency));

    adj->vertice = vertice;
    adj->weight = weight;
    adj->next = NULL;

    return adj;
}

void list_add_edge(void *vl, int v1, int v2, weight peso, int direction){
    if( v1 == v2 ) return;
    if(list_edge_exists(vl, v1, v2, direction)) return;

    List v = vl;
    Adjacency *adj = adjacency_create(v2, peso);
    adj->next = v[v1].head;
    v[v1].head = adj;

}

void list_remove_edge(void *vl, int v1, int v2, int direction){
    if( v1 == v2 ) return;
    if(!list_edge_exists(vl, v1, v2, direction)) return;

    List v = vl;
    Adjacency *adj = v[v1].head, *prev = NULL;
    while( adj && adj->vertice != v2 ){
        prev = adj;
        adj = adj->next;
    }
    
    if(adj){
        if(prev){
            prev->next = adj->next;
        } else {
            v[v1].head = adj->next;
        }
    }
    list_remove_edge(vl, v2, v1, UNDIRECTED);
}

char list_edge_exists(void *vl, int v1, int v2, int direction){
    if( v1 == v2 ) return 0;

    List v = vl;
    Adjacency *adj = v[v1].head;
    while(adj){
        if(adj->vertice == v2) return 1;
        adj = adj->next;
    }

    return (direction == DIRECTED) ? 0 : list_edge_exists(vl, v2, v1, DIRECTED);
}

void list_print(void *vl, int size){
    List v = vl;

    for(int i = 0; i < size; i++){
        printf("v%d: ", i);

        Adjacency *adj = v[i].head;
        while(adj){
            printf("v%d(%.1f) ", adj->vertice, adj->weight);
            adj = adj->next;
        }
        
        printf("\n");
    }
}

void list_destroy(void *vl, int size){
    List v = vl;
    for(int i = 0; i < size; i++){

        Adjacency *adj = v[i].head, *aux;

        /* Destruir data, no caso nao tem */
        // free(v[i].data);

        while(adj){
            aux = adj;
            adj = adj->next;
            free(aux);
        }
    }
    free(v);
}