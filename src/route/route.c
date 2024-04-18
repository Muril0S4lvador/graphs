#include "route.h"

struct Route{
    int size;
    int *route;
    double cost;
    int demand;
};

Route* route_construct(int size){
    Route *r = malloc(sizeof(Route) * size);
    return r;
}

int *route_return_route(Route *r){
    return r->route;
}

int route_return_size(Route *r){
    return r->size;
}

int route_return_demand(Route *r){
    return r->demand;
}

costType route_return_cost(Route *r){
    return r->cost;
}

void route_set_route(Route *r, int *route){
    r->route = malloc(sizeof(int) * sizeof(route));
    memcpy(r->route, route, sizeof(int) * sizeof(route));
}

void route_set_size(Route *r, int size){
    r->size = size;
}

void route_set_demand(Route *r, int demand){
    r->demand = demand;
}

void route_set_cost(Route *r, costType cost){
    r->cost = cost;
}

void route_print(Route *r){
    // for(int j = 0; j < size; j++){

    //     printf("#Route %d | Demand:%d Cost: %d\n", r->demand, r->cost);
    //     int size = r->size;
    //     for(int i = 0; i < size; i++){
    //         printf("%d ", r->route[i]);
    //     }
    //     printf("\n");
    // }
}
    
void route_destroy(Route *r){
    // for(int i = 0; i < size; i++)
        free(r->route);
}
