#ifndef _DATA_H_
#define _DATA_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct Data Data;

Data *data_construct(float x, float y, float demand);

void data_set_demand(Data *d, float demand);

float data_return_x(Data *d);

float data_return_y(Data *d);

float data_return_demand(Data *d);

void data_print(Data *d);

void data_destroy(Data *d);

#endif