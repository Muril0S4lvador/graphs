#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Route Route;
typedef unsigned char bool;
typedef int costType;

#define COST_PRINT "%d"

Route* route_construct(int size);

int *route_return_route(Route *r, int index);

int route_return_size(Route *r, int index);

int route_return_demand(Route *r, int index);

costType route_return_cost(Route *r, int index);

costType route_return_total_cost(Route *r, int size);

void route_set_route(Route *r, int *route, int index, int size);

void route_set_size(Route *r, int size, int index);

void route_set_demand(Route *r, int demand, int index);

void route_set_cost(Route *r, costType cost, int index);

void route_print(Route *r, int index);

void route_print_file(Route *r, int size, FILE *arq);
    
void route_destroy(Route *r, int size);

#endif