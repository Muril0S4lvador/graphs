#include "list.h"
#include "../algorithms/algorithms.h"

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

char list_add_edge(void *vl, int v1, int v2, weight peso){
    if(list_edge_exists(vl, v1, v2)) return 0;

    List v = vl;
    Adjacency *adj = adjacency_create(v2, peso);
    adj->next = v[v1].head;
    v[v1].head = adj;
    return 1;
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

void list_file_write(void *vl, int size, FILE *arq, char *edge){
    
    List l = vl;
    for(int i = 0; i < size; i++){
        Adjacency *aux = l[i].head;

        while(aux){
            fprintf(arq, "v%d %s v%d;\n", i, edge, aux->vertice);
            aux = aux->next;
        }
    }
}

void list_return_kruskal(void *vl, int sizeVertex, int sizeEdges, void *vk){
    List l = vl;
    Kruskal *k = vk;
    
    for(int i = 0, j = 0; j < sizeEdges ; j++){
        
        Adjacency *adj = l[i].head;

        while(adj){
            k[j].src = i;
            k[j].dest = adj->vertice;
            k[j].weight = adj->weight;

            if(k[j].src == 30 && k[j].dest == 31) printf("%d\n", j);

            adj = adj->next;
            if( adj ) j++;
        }
        
        if( i < sizeVertex ) i++;
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