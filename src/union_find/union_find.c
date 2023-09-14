#include "union_find.h"


UF* UF_init(uint16_t size) {
    UF* uf = malloc(sizeof(UF));
    uf->id = malloc(sizeof(uint16_t) * size);
    uf->size = malloc(sizeof(uint16_t) * size);
    uf->N = size;
    for (uint16_t i = 0; i < size; i++) {
        uf->id[i] = i;  // Cada objeto comeca na sua propria componente.
        uf->size[i] = 1;// N acessos ao array.
    }                   
    return uf;
}


uint16_t UF_find(UF* uf, uint16_t i) {
    while (i != uf->id[i]){
        uf->id[i] = uf->id[ uf->id[i] ];
        i = uf->id[i]; // Buscar o pai ate a raiz.
    }
    return i; // Profundidade de i acessos.
}

void UF_union(UF* uf, uint16_t p, uint16_t q) {
    uint16_t i = UF_find(uf, p); // Modifique raiz de p para a raiz de q.
    uint16_t j = UF_find(uf, q); // Profundidade de p+q acessos.
    if( i == j ) return;
    else if( uf->size[i] < uf->size[j] ){
        uf->id[i] = j;
        uf->size[j] += uf->size[i];
        
    } else {
        uf->id[j] = i;
        uf->size[i] += uf->size[j];
    }
}

int UF_connected(UF* uf, uint16_t p, uint16_t q) {
    return UF_find(uf, p) == UF_find(uf, q); // Verificar se p e q estao na mesma componente.
}

void UF_free(UF* uf) {
    free(uf->id);
    free(uf->size);
    free(uf);
}