#ifndef _INFO_H_
#define _INFO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../graph/graph.h"

typedef struct Info Info;

Info *info;

void info_construct(Graph *g, int seed);

void info_inc_total_iterations_vns();
void info_inc_imp_iterations_vns();
void info_inc_real_noimp_iterations_vns();

void info_inc_total_iterations_vnd();
void info_inc_imp_iterations_vnd();
void info_inc_noimp_iterations_vnd();

void info_set_time_constructive(clock_t start, clock_t end);
void info_set_time_enables     (clock_t start, clock_t end);
void info_set_time_vnd         (clock_t start, clock_t end);
void info_set_time_vns         (clock_t start, clock_t end);

void info_set_routes(Route *r);

void info_print();

void info_destroy();



#endif