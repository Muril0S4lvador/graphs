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

int *route_return_route(Route *r, int i){
    return r[i].route;
}

int route_return_size(Route *r, int i){
    return r[i].size;
}

int route_return_demand(Route *r, int i){
    return r[i].demand;
}

costType route_return_cost(Route *r, int i){
    return  r[i].cost;
}

void route_set_route(Route *r, int *route, int i, int size){
    r[i].route = malloc(sizeof(int) * size);
    memcpy(r[i].route, route, sizeof(int) * size);
}

void route_set_size(Route *r, int size, int i){
    r[i].size = size;
}

void route_set_demand(Route *r, int demand, int i){
    r[i].demand = demand;
}

void route_set_cost(Route *r, costType cost, int i){
    r[i].cost = cost;
}

void route_print(Route *r, int size){
    int sum = 0;
    for(int j = 0; j < size; j++){

        // printf("Route #%d | Demand:%d Cost: %d\n", j + 1, r[j].demand, (int)r[j].cost);
        printf("Route #%d: ", j + 1);
        int size = r[j].size;
        for(int i = 0; i < size; i++){
            printf("%d ", r[j].route[i]);
        }
        printf("\nDemand: %d Cost: %.0lf\n", r[j].demand, r[j].cost);
        sum += r[j].cost;
        printf("\n");
    }
    printf("Total Cost: %d\n", sum);
}
    
void route_destroy(Route *r, int size){
    for(int i = 0; i < size; i++)
        free(r[i].route);
}
