
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

static const int VECTOR_INIT_SIZE = 10;
static const int VECTOR_GROWTH_RATE = 2;

Vector *vector_construct()
{
    Vector *v = (Vector *)calloc(1, sizeof(Vector));

    v->allocated = VECTOR_INIT_SIZE;
    v->size = 0;
    v->data = (data_type *)calloc(VECTOR_INIT_SIZE, sizeof(data_type));

    return v;
}

int vector_size(Vector *v)
{
    return v->size;
}

data_type vector_get(Vector *v, int i)
{
    if (i >= vector_size(v))
    {
        printf("Error: vector_get: invalid index %d for vector with size %d.\n", i, vector_size(v));
        exit(0);
    }

    return v->data[i];
}

void vector_set(Vector *v, int i, data_type val)
{
    if (i >= vector_size(v))
    {
        printf("Error: vector_set: invalid index %d for vector with size %d.\n", i, vector_size(v));
        exit(0);
    }

    v->data[i] = val;
}

void vector_push_back(Vector *v, data_type val)
{
    if (v->size >= v->allocated)
    {
        v->allocated *= VECTOR_GROWTH_RATE;
        v->data = (data_type *)realloc(v->data, v->allocated * sizeof(data_type));
    }

    v->data[v->size] = val;
    v->size++;
}

int vector_find(Vector *v, data_type val)
{
    int i = 0;
    while (i < vector_size(v))
    {
        if (vector_get(v, i) == val)
            return i;
        i++;
    }

    return -1;
}

data_type vector_remove(Vector *v, int i)
{
    if (i >= v->size || i < 0)
    {
        printf("Error: vector_remove: invalid index %d for vector with size %d.\n", i, v->size);
        exit(0);
    }

    data_type val = v->data[i];

    for (int j = i; j < v->size - 1; j++)
        v->data[j] = v->data[j + 1];

    v->size--;

    return val;
}

data_type vector_pop_front(Vector *v)
{
    return vector_remove(v, 0);
}

data_type vector_pop_back(Vector *v)
{
    return vector_remove(v, v->size - 1);
}

void vector_insert(Vector *v, int i, data_type val)
{
    if (i < 0 || i > v->size)
    {
        printf("Error: vector_insert: invalid index %d for vector with size %d.\n", i, v->size);
        exit(0);
    }

    if (v->size == v->allocated)
    {
        v->allocated *= 2;
        v->data = (data_type *)realloc(v->data, v->allocated * sizeof(data_type));
    }

    for (int j = v->size; j > i; j--)
        v->data[j] = v->data[j - 1];

    v->data[i] = val;
    v->size++;
}

void vector_swap(Vector *v, int i, int j)
{
    if (i < 0 || i >= v->size || j < 0 || j >= v->size)
    {
        printf("Error: vector_swap: invalid indices %d and %d for vector with size %d.\n", i, j, v->size);
        exit(0);
    }

    data_type temp = v->data[i];
    v->data[i] = v->data[j];
    v->data[j] = temp;
}

void vector_sort(Vector *v)
{
    for (int i = 0; i < v->size - 1; i++)
    {
        int n_trocas = 0;

        for (int j = 0; j < v->size - i - 1; j++)
        {
            if (v->data[j] > v->data[j + 1])
            {
                vector_swap(v, j, j + 1);
                n_trocas++;
            }
        }

        if (n_trocas == 0)
            break;
    }
}

void vector_quick_sort_recursive(Vector *v, int st, int end)
{
    if (st >= end)
        return;

    int pivot = rand() % (end - st + 1) + st;
    int insert_point = st;

    vector_swap(v, pivot, end);

    for (int i = st; i <= end - 1; i++)
    {
        if (v->data[i] < v->data[pivot])
        {
            vector_swap(v, i, insert_point);
            insert_point++;
        }
    }

    vector_swap(v, pivot, insert_point);

    vector_quick_sort_recursive(v, st, insert_point - 1);
    vector_quick_sort_recursive(v, insert_point + 1, end);
}

void vector_quick_sort(Vector *v)
{
    vector_quick_sort_recursive(v, 0, v->size - 1);
}

int vector_binary_search(Vector *v, data_type val)
{
    int left = 0;
    int right = v->size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (v->data[mid] == val)
            return mid;
        else if (v->data[mid] < val)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

void vector_reverse(Vector *v)
{
    for (int i = 0; i < v->size / 2; i++)
        vector_swap(v, i, v->size - i - 1);
}

Vector *vector_copy(Vector *v)
{
    Vector *copy = (Vector *)malloc(sizeof(Vector));

    copy->data = (data_type *)malloc(v->allocated * sizeof(data_type));
    copy->size = v->size;
    copy->allocated = v->allocated;

    memcpy(copy->data, v->data, v->size * sizeof(data_type));

    return copy;
}

void vector_destroy(Vector *v)
{
    free(v->data);
    free(v);
}

void vector_clear(Vector *v)
{
    v->size = 0;
}

VectorIterator *vector_it_construct(Vector *v)
{
    VectorIterator *it = (VectorIterator *)malloc(sizeof(VectorIterator));
    it->v = v;
    it->i = 0;
    return it;
}

data_type vector_it_next(VectorIterator *it)
{
    data_type data = it->v->data[it->i];
    it->i++;
    return data;
}

bool vector_it_has_next(VectorIterator *it)
{
    return it->i < it->v->size;
}

void vector_it_destroy(VectorIterator *it)
{
    free(it);
}
