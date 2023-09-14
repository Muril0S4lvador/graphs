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

void list_add_edge(void *vl, int v1, int v2, weight peso){
    if(list_edge_exists(vl, v1, v2)) return;

    List v = vl;
    Adjacency *adj = adjacency_create(v2, peso);
    adj->next = v[v1].head;
    v[v1].head = adj;

}

void list_remove_edge(void *vl, int v1, int v2){
    
    if(!list_edge_exists(vl, v1, v2)) return;

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
}

char list_edge_exists(void *vl, int v1, int v2){

    List v = vl;
    Adjacency *adj = v[v1].head;
    while(adj){
        if(adj->vertice == v2) return 1;
        adj = adj->next;
    }

    return 0;
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

void _list_file_write(void *vl, int size, FILE *arq, char *edge){
    
    List l = vl;
    for(int i = 0; i < size; i++){
        Adjacency *aux = l[i].head;

        while(aux){
            fprintf(arq, "v%d %s v%d;\n", i, edge, aux->vertice);
            aux = aux->next;
        }
    }
}

void *_list_return_adjacencies(void *vl, int size, char direction){
    List l = vl;
    
    if( direction == UNDIRECTED ) size/2;
    
    for(int i = 0; i < size; i++){
        Adjacency *adj = l[i].head;

        while(adj){

        }
    }

    return NULL;
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