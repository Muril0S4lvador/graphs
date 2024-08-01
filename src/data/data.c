#include "data.h"

struct Data{
    float x, y;
    int demand,
        id;
};

Data *data_construct(float x, float y, int demand){
    Data *d = malloc(sizeof(Data));

    d->x = x;
    d->y = y;
    d->demand = demand;
    d->id = 0;

    return d;
}

void data_set_id(Data *d, int id){
    d->id = id;
}

void data_set_demand(Data *d, int demand){
    d->demand = demand;
}

float data_return_x(Data *d){
    return (d) ? d->x : 0 ;
}

float data_return_y(Data *d){
    return (d) ? d->y : 0 ;
}

int data_return_demand(Data *d){
    return (d) ? d->demand : 0 ;
}

void data_print(Data *d, FILE *arq){
    fprintf(arq, "(%.2f, %.2f) - %d\n", d->x, d->y, d->demand);
}

void data_destroy(Data *d){
    free(d);
}
