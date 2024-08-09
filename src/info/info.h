#ifndef _INFO_H_
#define _INFO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#include "../graph/graph.h"
#include "../Vector/vector.h"

typedef struct Info Info;

Info *info_malloc();
Info **info_array_construct();

void info_define(Info **arr, int idx);

void info_construct(Graph *g);

void info_inc_total_iterations_vns();
void info_inc_imp_iterations_vns();
void info_inc_real_noimp_iterations_vns();

void info_inc_total_iterations_vnd();
void info_inc_imp_iterations_vnd();
void info_inc_noimp_iterations_vnd();

void info_inc_move_perturbation();
void info_inc_random_perturbation();
void info_inc_cross_perturbation();

void info_set_time_constructive(clock_t start, clock_t end);
void info_set_time_enables     (clock_t start, clock_t end);
void info_set_time_vnd         (clock_t start, clock_t end);
void info_set_time_vns         (clock_t start, clock_t end);

void info_set_cost_constructive(int cost);
void info_set_cost_enables     (int cost);
void info_set_cost_vns         (int cost);

void info_save_improvement_vns(int value);

void info_set_routes(Route *r);

void info_set_seed(int seed);

void info_print();

void info_print_arr_file(Info **arr, int size);

void info_print_solution_file(Info **arr, int size);

void info_print_results_file(Info **arr, int size);

void info_print_vectors(Vector *improv, Vector *it_improv, int seed, char *instance, int control);

void info_print1(Info *info1);

void info_print_table_result(Info **arr, int size);
void info_print_table_infos (Info **arr, int size);

void info_reset();

void info_route_destroy();

void info_arr_destroy(Info **arr, int size);

void info_destroy();



void info_print_ERI(Info **arr, int size);

#endif