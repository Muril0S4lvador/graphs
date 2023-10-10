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
        free(adj);
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

void list_return_edges(void *vl, int sizeVertex, int sizeEdges, void *vk){
    List l = vl;
    Edges *k = vk;
    
    for(int i = 0, j = 0; j < sizeEdges ; j++){
        
        Adjacency *adj = l[i].head;

        while(adj){
            k[j].src = i;
            k[j].dest = adj->vertice;
            k[j].weight = adj->weight;

            adj = adj->next;
            if( adj ) j++;
        }
        
        if( i < sizeVertex ) i++;
    }
}

void list_return_edges_cost(void *vl, int sizeVertex, void *vk, void *vn){
    List l = vl;
    Edges *e = vk,
          *near_0 = vn;
    int sizeE = 0;
    
    Adjacency *adj_i, *adj_j, *adj_ij;
    char ctli, ctlj, ctlij, ctl;

    for(int i = 1; i < sizeVertex; i++){

        adj_i = l[0].head;

        // j é sempre > i
        for(int j = i + 1; j < sizeVertex; j++){
        
            adj_ij = l[i].head;
            adj_j = l[0].head;
            ctli = ctlj = ctlij = ctl = 0;

            while(1){

                if( adj_i->vertice  != i )  adj_i = adj_i->next;   else ctli++;
                if( adj_j->vertice  != j )  adj_j = adj_j->next;   else ctlj++;
                if( adj_ij->vertice != j ) adj_ij = adj_ij->next;  else ctlij++;

                if( ctli && ctlj && ctlij ) break;

                if( !adj_i || !adj_ij || !adj_j ){ ctl = 1; break; }

            }

            if( !ctl ){
                e[sizeE].src = i;
                e[sizeE].dest = j;
                e[sizeE].weight = (adj_i->weight + adj_j->weight) - adj_ij->weight;
                sizeE++;
            }
        }
    }

    adj_i = l[0].head;
    int i = 0;
    while(adj_i){
        near_0[i].src = 0;
        near_0[i].dest = adj_i->vertice;
        near_0[i].weight = adj_i->weight;
        i++;
        adj_i = adj_i->next;
    }
}

void list_dfs_recursive(void *vl, int *route, int *size_route, int *visited){
    List l = vl;
    Adjacency *adj = l[0].head;
    route[(*size_route)++] = 0;
    visited[0] = 1;
    _list_dfs_recursive(adj, vl, route, size_route, visited);
}

void _list_dfs_recursive(Adjacency *adj, void *vl, int *route, int *size_route, int *visited){
    List l = vl;
    if( !adj ) return;
    if( !visited[adj->vertice] ){

        route[(*size_route)++] = adj->vertice;
        visited[adj->vertice] = 1;
        _list_dfs_recursive(l[adj->vertice].head, vl, route, size_route, visited);
    }
    _list_dfs_recursive(adj->next, vl, route, size_route, visited);

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