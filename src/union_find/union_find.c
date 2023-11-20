#include "union_find.h"

UF* UF_init(int size) {
    UF* uf = malloc(sizeof(UF));
    uf->id = malloc(sizeof(int) * size);
    uf->size = malloc(sizeof(int) * size);
    uf->N = size;
    for (int i = 0; i < size; i++) {
        uf->id[i] = i;  // Cada objeto comeca na sua propria componente.
        uf->size[i] = 1;// N acessos ao array.
    }                   
    return uf;
}


int UF_find(UF* uf, int i) {
    while (i != uf->id[i]){
        uf->id[i] = uf->id[ uf->id[i] ];
        i = uf->id[i]; // Buscar o pai ate a raiz.
    }
    return i; // Profundidade de i acessos.
}

void UF_union(UF* uf, int p, int q) {
    int i = UF_find(uf, p); // Modifique raiz de p para a raiz de q.
    int j = UF_find(uf, q); // Profundidade de p+q acessos.
    if( i == j ) return;
    else if( uf->size[i] < uf->size[j] ){
        uf->id[i] = j;
        uf->size[j] += uf->size[i];
        
    } else {
        uf->id[j] = i;
        uf->size[i] += uf->size[j];
    }
}

int UF_connected(UF* uf, int p, int q) {
    return UF_find(uf, p) == UF_find(uf, q); // Verificar se p e q estao na mesma componente.
}

void UF_free(UF* uf) {
    free(uf->id);
    free(uf->size);
    free(uf);
}