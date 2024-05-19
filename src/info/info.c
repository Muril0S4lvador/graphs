#include "info.h"

struct Info{
    char *instance;
    int optimal;
    int capacity;

    int cost_constructive;
    int cost_enables;
    int cost_VNS;

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

    Vector *improvements_vns_vector;
    Vector *it_improvements_vns_vector;
};

Info *info;

/* =============================================== FUNÇÕES INTERNAS ================================================================== */

double _calculate_time(clock_t start, clock_t end){
    return ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
}

void _print_vector_file(int *vet, int size, FILE *arq){
    if(!size){ fprintf(arq, "--\n"); return; }
    for(int i = 0; i < size; i++){
        fprintf(arq, "%d", vet[i]);
        if(i < size - 1) fprintf(arq, ", ");
    }
    fprintf(arq, "\n");
}

void _directory_verify(){
    if(!info || !info->instance){printf("ERRO: No instance\n"); return;}

    DIR *dir = opendir("out/");
    if( dir ){
        closedir( dir );
    } else {
        system("make out/");
    }

    char directory_group[50];
    sprintf(directory_group, "out/%c/", info->instance[0]);
    dir = opendir(directory_group);
    if( dir ){
        closedir( dir );
    } else {
        char call[60];
        sprintf(call, "mkdir %s", directory_group);
        system(call);
    }
    
    char directory[50];
    sprintf(directory, "out/%c/%s", info->instance[0], info->instance);
    dir = opendir(directory);
    if( dir ){
        closedir( dir );
    } else {
        char call[60];
        sprintf(call, "mkdir %s", directory);
        system(call);
    }
}

/* =================================================================================================================================== */


Info *info_malloc(){
    Info *i = malloc(sizeof(Info));
    return i;
}

void info_define(Info **arr, int idx){
    info = arr[idx];
}

Info **info_array_construct(int size){
    Info **arr = malloc(sizeof(Info*) * size);
    for(int i = 0; i < size; i++)
        arr[i] = info_malloc();
    return arr;
}

void info_arr_destroy(Info **arr, int size){
    for(int i = 0; i < size; i++){
        info_define(arr, i);
        info_destroy();
    }
    free(arr);
}

void info_construct(Graph *g){
    if(!info) info = info_malloc();

    info->total_iterations_vns = 0;
    info->imp_iterations_vns = 0;
    info->noImp_iterations_vns = NUM_IT;
    info->real_noImp_iterations_vns = 0;

    info->total_iterations_vnd = 0;
    info->imp_iterations_vnd = 0;
    info->noImp_iterations_vnd = 0;

    info->cost_constructive = 0;
    info->cost_enables = 0;
    info->cost_VNS = 0;

    info->neighborhood_structures = NEIGHBORHOOD_STRUCTURES;
    info->srand_seed = -1;

    info->time_ms_constructive = 0;
    info->time_ms_enables = 0;
    info->time_ms_vnd = 0;
    info->time_ms_vns = 0;

    info->instance = malloc(sizeof(char) * (strlen(graph_return_name(g)) + 1) );
    strcpy(info->instance, graph_return_name(g));
    info->optimal = graph_return_optimal_cost(g);
    info->num_routes = graph_return_trucks(g);
    info->capacity = graph_return_capacity(g);

    info->routes = NULL;

    info->improvements_vns_vector = vector_construct();
    info->it_improvements_vns_vector = vector_construct();
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

double info_return_total_time(Info *i){
    return i->time_ms_constructive + i->time_ms_enables + i->time_ms_vns;
}

void info_set_cost_constructive(int cost){
    info->cost_constructive = cost;
}
void info_set_cost_enables(int cost){
    info->cost_enables = cost;
}
void info_set_cost_vns(int cost){
    info->cost_VNS = cost;
}

void info_save_improvement_vns(int value){
    int *val = malloc(sizeof(int)), *it = malloc(sizeof(int));
    *val = value; *it = info->total_iterations_vns;
    vector_push_back(info->improvements_vns_vector, val);
    vector_push_back(info->it_improvements_vns_vector, it);
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

void info_set_seed(int seed){
    info->srand_seed = seed;    
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

void info_print1(Info *info1){

    if(!info1 || !info1->instance ){ printf("ERROR: No instance\n"); return; }

    printf("\nInstance %s (Opt: %d)\n", info1->instance, info1->optimal);

    printf("\nNeighborhood structures: %d\n", info1->neighborhood_structures);
    printf("Srand seed: %d\n\n", info1->srand_seed);
    
    printf("VNS\n");
    printf("Total iterations:               %d\n", info1->total_iterations_vns);
    printf("No improvement iterations:      %d\n", info1->noImp_iterations_vns);
    printf("Improvement iterations:         %d\n", info1->imp_iterations_vns);
    printf("Real no improvement iterations: %d\n", info1->real_noImp_iterations_vns);

    printf("\nVND\n");
    printf("Total iterations:               %d\n", info1->total_iterations_vnd);
    printf("No improvement iterations:      %d\n", info1->noImp_iterations_vnd);
    printf("Improvement iterations:         %d\n", info1->imp_iterations_vnd);

    printf("\n");

    printf("ALGORITHMS\n");
    printf("Constructive time:   %.4lf ms\n", info1->time_ms_constructive);
    printf("Enables routes time: %.4lf ms\n", info1->time_ms_enables);
    printf("VND time:            %.4lf ms\n", info1->time_ms_vnd);
    printf("VNS time:            %.4lf ms\n", info1->time_ms_vns);

    if(!info1->routes) return;
    printf("Cost: %d\n", route_return_total_cost(info1->routes, info1->num_routes));
    printf("\n");
}

void info_print_arr_file(Info **arr, int size){

    _directory_verify();
    char filename[100];
    FILE *arq;
    
    for(int i = 0; i < size; i++){

        if(!arr[i] || !arr[i]->instance ){ printf("ERROR: No instance\n"); return; }

        sprintf(filename, "out/%c/%s/%s_Seed%d.info", arr[i]->instance[0], arr[i]->instance, arr[i]->instance, arr[i]->srand_seed);
        arq = fopen(filename, "w");

        if(!arq) {printf("ERRO: Problem with file %s\n", filename); continue;}   

        fprintf(arq, "Instance %s (Opt: %d)\n", arr[i]->instance, arr[i]->optimal);

        fprintf(arq, "\nNeighborhood structures: %d\n", arr[i]->neighborhood_structures);
        fprintf(arq, "Srand seed: %d\n\n", arr[i]->srand_seed);
        
        fprintf(arq, "VNS\n");
        fprintf(arq, "Total iterations:               %d\n", arr[i]->total_iterations_vns);
        fprintf(arq, "No improvement iterations:      %d\n", arr[i]->noImp_iterations_vns);
        fprintf(arq, "Improvement iterations:         %d\n", arr[i]->imp_iterations_vns);
        fprintf(arq, "Real no improvement iterations: %d\n", arr[i]->real_noImp_iterations_vns);

        fprintf(arq, "\nVND\n");
        fprintf(arq, "Total iterations:               %d\n", arr[i]->total_iterations_vnd);
        fprintf(arq, "No improvement iterations:      %d\n", arr[i]->noImp_iterations_vnd);
        fprintf(arq, "Improvement iterations:         %d\n", arr[i]->imp_iterations_vnd);

        fprintf(arq, "\n");

        fprintf(arq, "ALGORITHMS\n");
        fprintf(arq, "Constructive time:   %.4lf ms\n", arr[i]->time_ms_constructive);
        fprintf(arq, "Enables routes time: %.4lf ms\n", arr[i]->time_ms_enables);
        fprintf(arq, "VND time:            %.4lf ms\n", arr[i]->time_ms_vnd);
        fprintf(arq, "VNS time:            %.4lf ms\n", arr[i]->time_ms_vns);

        if(!arr[i]->routes) return;

        fprintf(arq, "\nCost: %d\n", route_return_total_cost(arr[i]->routes, arr[i]->num_routes));

        fclose(arq);
        arq = NULL;

        info_print_vectors(arr[i]->improvements_vns_vector, arr[i]->it_improvements_vns_vector, info->srand_seed, info->instance, 1);
    }
}

void info_print_solution_file(Info **arr, int size){

    _directory_verify();
    char filename[100];
    sprintf(filename, "out/%c/%s/%s.sol", arr[0]->instance[0], arr[0]->instance, arr[0]->instance);
    FILE *arq = fopen(filename, "w");
    if(!arq) {printf("ERRO: Problem with file %s\n", filename); return;} 
    
    int best = 0,
        cost = route_return_total_cost(arr[0]->routes, arr[0]->num_routes);
    for( int i = 0; i < size; i++){
        int newCost = route_return_total_cost(arr[i]->routes, arr[i]->num_routes);
        if( newCost < cost ){
            cost = newCost;
            best = i;
        }
    }

    route_print_file(arr[best]->routes, arr[best]->num_routes, arq);
    fprintf(arq, "Optimal cost: %d\n", arr[best]->optimal);
    fprintf(arq, "\nCapacity: %d\n", arr[best]->capacity);
    fprintf(arq, "Srand Seed: %d\n", arr[best]->srand_seed);
    fclose(arq);
}

void info_print_results_file(Info **arr, int size){

    if(!info || !info->instance ){ printf("ERROR: No instance\n"); return; }
    if(!info->routes){ printf("ERROR: No results\n"); return; }
    _directory_verify();

    int med_total_it_vns = 0,
        med_imp_it_vns = 0,
        med_real_noImp_it_vns = 0,
        med_total_it_vnd = 0,
        med_imp_it_vnd = 0,
        med_noImp_it_vnd = 0,
        med_cost = 0,
        tam = arr[0]->num_routes,
        vet_opt[tam],
        vet_Nopt[tam],
        size_opt = 0,
        size_Nopt = 0,
        min_cost = route_return_total_cost(arr[0]->routes, arr[0]->num_routes),
        max_cost = route_return_total_cost(arr[0]->routes, arr[0]->num_routes);

    double med_time_ms_constructive = 0,
           med_time_ms_enables = 0,
           med_time_ms_vnd = 0,
           med_time_ms_vns = 0;
    
    for(int i = 0; i < size; i++){
        med_total_it_vns += arr[i]->total_iterations_vns;
        med_imp_it_vns += arr[i]->imp_iterations_vns;
        med_real_noImp_it_vns += arr[i]->real_noImp_iterations_vns;

        med_total_it_vnd += arr[i]->total_iterations_vnd;
        med_imp_it_vnd += arr[i]->imp_iterations_vnd;
        med_noImp_it_vnd += arr[i]->noImp_iterations_vnd;
        
        med_time_ms_constructive += arr[i]->time_ms_constructive;
        med_time_ms_enables += arr[i]->time_ms_enables;
        med_time_ms_vnd += arr[i]->time_ms_vnd;
        med_time_ms_vns += arr[i]->time_ms_vns;

        int total_cost = route_return_total_cost(arr[i]->routes, arr[i]->num_routes);
        med_cost += total_cost;

        min_cost = (total_cost < min_cost) ? total_cost : min_cost;
        max_cost = (total_cost > max_cost) ? total_cost : max_cost;

        if(total_cost == arr[i]->optimal)
            vet_opt[size_opt++] = arr[i]->srand_seed;
        else
            vet_Nopt[size_Nopt++] = arr[i]->srand_seed;
    }

    med_total_it_vns /= size;
    med_imp_it_vns /= size;
    med_real_noImp_it_vns /= size;
    med_total_it_vnd /= size;
    med_imp_it_vnd /= size;
    med_noImp_it_vnd /= size;
    med_time_ms_constructive /= size;
    med_time_ms_enables /= size;
    med_time_ms_vnd /= size;
    med_time_ms_vns /= size;
    med_cost /= size;

    char filename[50];
    sprintf(filename, "out/%c/%s/%s_results.info", arr[0]->instance[0], arr[0]->instance, arr[0]->instance);
    FILE *arq = fopen(filename, "w");
    if(!arq) {printf("ERRO: Problem with file %s\n", filename); return;} 

    fprintf(arq, "Instance %s (Opt: %d)\n", arr[0]->instance, arr[0]->optimal);
    fprintf(arq, "\nTimes tested: %d\n", size);

    fprintf(arq, "\nVNS\n");
    fprintf(arq, "Average total iterations:               %d\n", med_total_it_vns);
    fprintf(arq, "Average improvement iterations:         %d\n", med_imp_it_vns);
    fprintf(arq, "Average real no improvement iterations: %d\n", med_real_noImp_it_vns);

    fprintf(arq, "\nVND\n");
    fprintf(arq, "Average total iterations:               %d\n", med_total_it_vnd);
    fprintf(arq, "Average improvement iterations:         %d\n", med_imp_it_vnd);
    fprintf(arq, "Average no improvement iterations:      %d\n", med_noImp_it_vnd);
    fprintf(arq, "\nALGORITHMS\n");
    fprintf(arq, "Average constructive time:   %.4lf ms\n", med_time_ms_constructive);
    fprintf(arq, "Average enables routes time: %.4lf ms\n", med_time_ms_enables);
    fprintf(arq, "Average VND time:            %.4lf ms\n", med_time_ms_vnd);
    fprintf(arq, "Average VNS time:            %.4lf ms\n", med_time_ms_vns);

    fprintf(arq, "\nMinimun Cost: %d\n", min_cost);
    fprintf(arq, "Maximun Cost: %d\n", max_cost);
    fprintf(arq, "Average Cost: %d\n", med_cost);

    fprintf(arq, "\nOptimal srand seeds   : ");
    _print_vector_file(vet_opt, size_opt, arq);
    fprintf(arq, "No optimal srand seeds: ");
    _print_vector_file(vet_Nopt, size_Nopt, arq);

    fclose(arq);
}

void info_print_vectors(Vector *improv, Vector *it_improv, int seed, char *instance, int control){

    _directory_verify();
    char filename[100];
    sprintf(filename, "out/%c/%s/%s_Seed%dVNS.txt", instance[0], instance, instance, seed);
    FILE *arq = fopen(filename, "w");
    if(!arq) {printf("ERRO: Problem with file %s\n", filename); return;}

    if( vector_size(improv) != vector_size(it_improv) ){
        printf("NÃO É O MESMO VALOR! MAS DEVERIA\n");
        exit(EXIT_FAILURE);
    }

    // fprintf(arq, "x y\n");
    while(vector_size(improv)){
        int *it = vector_pop_front(it_improv), *val = vector_pop_front(improv);
        fprintf(arq, "%d %d\n", *it, *val );
        free(it);
        free(val);
    }
    fclose(arq);
}

void info_print_table_result(Info **arr, int size){

    FILE *arq = fopen("TableResults.txt", "a");
    if(!arq) return;
    
    int best = 0,
        cost = route_return_total_cost(arr[0]->routes, arr[0]->num_routes);
    double time = info_return_total_time(arr[0]);
    for( int i = 0; i < size; i++){
        int newCost = route_return_total_cost(arr[i]->routes, arr[i]->num_routes);
        double newTime = info_return_total_time(arr[i]);
        if( newCost < cost ){
            cost = newCost;
            best = i;
            time = newTime;
        } else if( newCost == cost ){
            if( newTime < time ){
                best = i;
                time = newTime;
            }
        }
    }

    if( cost == arr[best]->optimal ){

        fprintf(arq, "\\textbf{%s} & %d & %d & \\textbf{%d} & \\textbf{%d}& \\textbf{%.2lf} & \\textbf{%d} \\\\\n",
        arr[best]->instance, arr[best]->cost_constructive, arr[best]->cost_enables,
        arr[best]->cost_VNS, arr[best]->optimal, info_return_total_time(arr[best]), 
        arr[best]->srand_seed);
    } else {

        fprintf(arq, "%s & %d & %d & %d & %d & %.2lf & %d \\\\\n",
        arr[best]->instance, arr[best]->cost_constructive, arr[best]->cost_enables,
        arr[best]->cost_VNS, arr[best]->optimal, info_return_total_time(arr[best]), 
        arr[best]->srand_seed);
    }

    fclose(arq);
}
void info_print_table_infos(Info **arr, int size){

    FILE *arq = fopen("TableInfos.txt", "a");
    if(!arq) return;

    int max_cost = route_return_total_cost(arr[0]->routes, arr[0]->num_routes),
        min_cost = max_cost,
        med_num_it = 0,
        med_num_it_opt = 0;

    double time = 0,
           med_cost = 0,
           num_sucess = 0;

    for(int i = 0; i < size; i++){
        int total_cost = route_return_total_cost(arr[i]->routes, arr[i]->num_routes);
        med_cost += total_cost;
        min_cost = (total_cost < min_cost) ? total_cost : min_cost;
        max_cost = (total_cost > max_cost) ? total_cost : max_cost;

        if( total_cost == arr[i]->optimal ) num_sucess++;

        med_num_it += arr[i]->total_iterations_vns;

        int sizeV = vector_size(arr[i]->it_improvements_vns_vector) - 1,
            *numget;
        if( sizeV >= 0 ){
            numget = (int*)vector_get(arr[i]->it_improvements_vns_vector, sizeV);
            med_num_it_opt += *numget;
        } 

        time += info_return_total_time(arr[i]);
    }

    med_cost = round(med_cost / (double)size);
    med_num_it = round(med_num_it / (double)size);;
    med_num_it_opt = round(med_num_it_opt / (double)size);;
    time /= size;
    num_sucess = (num_sucess / (double)size) * 100;

    if( min_cost == arr[0]->optimal ){
        fprintf(arq, "\\textbf{%s} & %d & \\textbf{%d} & %.2lf & \\textbf{%.2lf} & %d & \\textbf{%d} & \\textbf{%.2lf}\\\\\n",
        arr[0]->instance, max_cost, min_cost, med_cost, num_sucess, med_num_it, med_num_it_opt, time);
    } else {
        fprintf(arq, "%s & %d & %d & %.2lf & -- & %d & -- & %.2lf\\\\\n",
        arr[0]->instance, max_cost, min_cost, med_cost, med_num_it, time);
    }

    fclose(arq);
}

void info_reset(){
    info->total_iterations_vns = 0;
    info->imp_iterations_vns = 0;
    info->real_noImp_iterations_vns = 0;

    info->total_iterations_vnd = 0;
    info->imp_iterations_vnd = 0;
    info->noImp_iterations_vnd = 0;

    info->time_ms_constructive = 0;
    info->time_ms_enables = 0;
    info->time_ms_vnd = 0;
    info->time_ms_vns = 0;
}

void info_route_destroy(){
    if(info->routes) 
        route_destroy(info->routes, info->num_routes);
}

void info_destroy(){
    info_route_destroy();
    vector_destroy(info->improvements_vns_vector);
    vector_destroy(info->it_improvements_vns_vector);
    free(info->instance);
    free(info);
}