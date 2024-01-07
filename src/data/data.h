#ifndef _DATA_H_
#define _DATA_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct Data Data;

// Constrói um elemento Data
Data *data_construct(float x, float y, int demand);

// Atribui uma demanda para um elemento Data
void data_set_demand(Data *d, int demand);

// Retorna o atributo x de um elemento Data
float data_return_x(Data *d);

// Retorna o atributo y de um elemento Data
float data_return_y(Data *d);

// Retorna o atributo demand de um elemento Data
int data_return_demand(Data *d);

// Imprime os atributos de um elemento Data
void data_print(Data *d);

// Desaloca da memória um elemento Data
void data_destroy(Data *d);

#endif