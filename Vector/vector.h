
#ifndef _DATA_STRUCTURES_VECTOR_H_
#define _DATA_STRUCTURES_VECTOR_H_

typedef void *data_type;
typedef unsigned char bool;

typedef struct
{
    data_type *data;
    int size;
    int allocated;
} Vector;

typedef struct
{
    Vector *v;
    int i;
} VectorIterator;

VectorIterator *vector_it_construct(Vector *v);
data_type vector_it_next(VectorIterator *it);
bool vector_it_has_next(VectorIterator *it);
void vector_it_destroy(VectorIterator *it);

// Cria um vetor vazio
Vector *vector_construct();

// Adiciona um elemento no final do vetor
void vector_push_back(Vector *v, data_type val);

// Retorna o i-ésimo elemento do vetor
data_type vector_get(Vector *v, int i);

// Atribui o valor val ao i-ésimo elemento do vetor
void vector_set(Vector *v, int i, data_type val);

// Retorna o tamanho do vetor
int vector_size(Vector *v);

// Retorna o indice do primeiro elemento com valor val. Retorna -1 se nao encontrado.
int vector_find(Vector *v, data_type val);

// Remove o i-ésimo elemento do vetor.
data_type vector_remove(Vector *v, int i);

// Remove o primeiro elemento
data_type vector_pop_front(Vector *v);

// Remove o ultimo elemento
data_type vector_pop_back(Vector *v);

// Insere o elemento na i-esima posicao
void vector_insert(Vector *v, int i, data_type val);

// Troca os elementos das posições i e j (i vira j e j vira i)
void vector_swap(Vector *v, int i, int j);

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v);

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val);

// Inverte o vetor in-place (sem criar um novo vetor)
void vector_reverse(Vector *v);

// Cria uma cópia do vector e dos valores de seus atributos.
Vector *vector_copy(Vector *v);

// Remove todos os elementos de v
void vector_clear(Vector *v);

// Libera a memoria alocada para o vetor
void vector_destroy(Vector *v);

#endif
