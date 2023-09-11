#include "data.h"

struct Data{
    int x, y;
    float demand;
};

Data *data_construct(int x, int y, float demand){
    Data *d = malloc(sizeof(Data));

    d->x = x;
    d->y = y;
    d->demand = demand;

    return d;
}

void data_set_demand(Data *d, float demand){
    d->demand = demand;
}

int data_return_x(Data *d){
    return (d) ? d->x : 0 ;
}

int data_return_y(Data *d){
    return (d) ? d->y : 0 ;
}

float data_return_demand(Data *d){
    return (d) ? d->demand : 0 ;
}

void data_print(Data *d){
    printf("(%d, %d) - %.1f\n", d->x, d->y, d->demand);
}

void data_destroy(Data *d){
    free(d);
}
