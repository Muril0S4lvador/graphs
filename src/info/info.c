#include "info.h"

struct Info{
    char *instance;
    int optimal;

    int total_iterations_vns;
    int imp_iterations_vns;
    int noImp_iterations_vns;
    int real_noImp_iterations_vns;
    
    int total_iterations_vnd;
    int imp_iterations_vnd;
    int noImp_iterations_vnd;
    
    int neighborhood_structures;
    int srand_seed;
    double time_ms_constructive;
    double time_ms_enables;
    double time_ms_vnd;
    double time_ms_vns;
    Route *routes;
    int num_routes;
};

double _calculate_time(clock_t start, clock_t end){
    return ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
}

void info_construct(Graph *g, int seed){
    info = malloc(sizeof(Info));

    info->total_iterations_vns = 0;
    info->imp_iterations_vns = 0;
    info->noImp_iterations_vns = NUM_IT;
    info->real_noImp_iterations_vns = 0;

    info->total_iterations_vnd = 0;
    info->imp_iterations_vnd = 0;
    info->noImp_iterations_vnd = 0;

    info->neighborhood_structures = NEIGHBORHOOD_STRUCTURES;
    info->srand_seed = seed;

    info->time_ms_constructive = 0;
    info->time_ms_enables = 0;
    info->time_ms_vnd = 0;
    info->time_ms_vns = 0;

    info->instance = malloc(sizeof(char) * (strlen(graph_return_name(g)) + 1) );
    strcpy(info->instance, graph_return_name(g));
    info->optimal = graph_return_optimal_cost(g);
    info->num_routes = graph_return_trucks(g);

    info->routes = NULL;
}

void info_inc_total_iterations_vns(){
    info->total_iterations_vns++;
}
void info_inc_imp_iterations_vns(){
    info->imp_iterations_vns++;
}
void info_inc_real_noimp_iterations_vns(){
    info->real_noImp_iterations_vns++;
}

void info_inc_total_iterations_vnd(){
    info->total_iterations_vnd++;
}
void info_inc_imp_iterations_vnd(){
    info->imp_iterations_vnd++;
}
void info_inc_noimp_iterations_vnd(){
    info->noImp_iterations_vnd++;
}

void info_set_time_constructive(clock_t start, clock_t end){
    info->time_ms_constructive = _calculate_time(start, end);
}
void info_set_time_enables(clock_t start, clock_t end){
    info->time_ms_enables += _calculate_time(start, end);
}
void info_set_time_vnd(clock_t start, clock_t end){
    info->time_ms_vnd += _calculate_time(start, end);
}
void info_set_time_vns(clock_t start, clock_t end){
    info->time_ms_vns = _calculate_time(start, end);
}

void info_set_routes(Route *r){
    info->routes = route_construct(info->num_routes);
    for(int i = 0; i < info->num_routes; i++){
        route_set_route(info->routes, route_return_route(r, i), i, route_return_size(r, i));
        route_set_cost(info->routes, route_return_cost(r, i), i);
        route_set_size(info->routes, route_return_size(r, i), i);
        route_set_demand(info->routes, route_return_demand(r, i), i);
    }
}

void info_print(){

    if(!info || !info->instance ){ printf("ERROR: No instance\n"); return; }

    printf("\nInstance %s (Opt: %d)\n", info->instance, info->optimal);

    printf("\nNeighborhood structures: %d\n", info->neighborhood_structures);
    printf("Srand seed: %d\n\n", info->srand_seed);
    
    printf("VNS\n");
    printf("Total iterations:               %d\n", info->total_iterations_vns);
    printf("No improvement iterations:      %d\n", info->noImp_iterations_vns);
    printf("Improvement iterations:         %d\n", info->imp_iterations_vns);
    printf("Real no improvement iterations: %d\n", info->real_noImp_iterations_vns);

    printf("\nVND\n");
    printf("Total iterations:               %d\n", info->total_iterations_vnd);
    printf("No improvement iterations:      %d\n", info->noImp_iterations_vnd);
    printf("Improvement iterations:         %d\n", info->imp_iterations_vnd);

    printf("\n");


    printf("ALGORITHMS\n");
    printf("Constructive time:   %.4lf ms\n", info->time_ms_constructive);
    printf("Enables routes time: %.4lf ms\n", info->time_ms_enables);
    printf("VND time:            %.4lf ms\n", info->time_ms_vnd);
    printf("VNS time:            %.4lf ms\n", info->time_ms_vns);

    if(!info->routes) return;
    printf("Cost: %d\n", route_return_total_cost(info->routes, info->num_routes));
    printf("\n");
}

void info_destroy(){
    route_destroy(info->routes, info->num_routes);
    free(info->instance);
    free(info);
}

