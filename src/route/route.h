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

int *route_return_route(Route *r, int i);

int route_return_size(Route *r, int i);

int route_return_demand(Route *r, int i);

costType route_return_cost(Route *r, int i);

void route_set_route(Route *r, int *route, int i, int size);

void route_set_size(Route *r, int size, int i);

void route_set_demand(Route *r, int demand, int i);

void route_set_cost(Route *r, costType cost, int i);

void route_print(Route *r, int i);
    
void route_destroy(Route *r, int size);

#endif