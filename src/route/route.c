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

int *route_return_route(Route *r, int index){
    return r[index].route;
}

int route_return_size(Route *r, int index){
    return r[index].size;
}

int route_return_demand(Route *r, int index){
    return r[index].demand;
}

costType route_return_cost(Route *r, int index){
    return  r[index].cost;
}

costType route_return_total_cost(Route *r, int size){
    if(!r) return -1;
    costType cost = 0;
    for(int i = 0; i < size; i++)
        cost += route_return_cost(r, i);
    return cost;
}

void route_set_route(Route *r, int *route, int index, int size){
    r[index].route = malloc(sizeof(int) * size);
    memcpy(r[index].route, route, sizeof(int) * size);
}

void route_set_size(Route *r, int size, int index){
    r[index].size = size;
}

void route_set_demand(Route *r, int demand, int index){
    r[index].demand = demand;
}

void route_set_cost(Route *r, costType cost, int index){
    r[index].cost = cost;
}

void route_print(Route *r, int size){
    int sum = 0;
    for(int j = 0; j < size; j++){

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

void route_print_file(Route *r, int size, FILE *arq){
    int sum = 0;
    for(int j = 0; j < size; j++){

        fprintf(arq, "Route #%d: ", j + 1);
        int size = r[j].size;
        for(int i = 0; i < size; i++){
            fprintf(arq, "%d ", r[j].route[i]);
        }
        fprintf(arq, "\nDemand: %d Cost: %.0lf\n\n", r[j].demand, r[j].cost);
        sum += r[j].cost;
    }
    fprintf(arq, "Total Cost: %d\n", sum);
}
    
void route_destroy(Route *r, int size){
    for(int i = 0; i < size; i++)
        free(r[i].route);
    free(r);
}
