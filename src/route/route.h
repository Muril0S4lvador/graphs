#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char bool;
typedef int costType;

#define COST_PRINT "%d"

typedef struct Route Route;

Route* route_construct(int size);

int *route_return_route(Route *r);

int route_return_size(Route *r);

int route_return_demand(Route *r);

costType route_return_cost(Route *r);

void route_set_route(Route *r, int *route);

void route_set_size(Route *r, int size);

void route_set_demand(Route *r, int demand);

void route_set_cost(Route *r, costType cost);

void route_print(Route *r);
    
void route_destroy(Route *r);

#endif