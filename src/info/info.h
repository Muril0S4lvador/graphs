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

// Realiza malloc de uma estrutura Info e a retorna
Info *info_malloc();

// Constrói um array de Info
Info **info_array_construct();

// Define qual será o indice de info usado como variável global
void info_define(Info **arr, int idx);

// Constrói um elemento de Info
void info_construct(Graph *g);

// Incrementa quantidade total de iterações do VNS
void info_inc_total_iterations_vns();

// Incrementa quantidade de iterações com melhora do VNS
void info_inc_imp_iterations_vns();

// Incrementa quantidade de iterações sem melhora do VNS
void info_inc_real_noimp_iterations_vns();

// Incrementa quantidade total de iterações do VND
void info_inc_total_iterations_vnd();

// Incrementa quantidade de iterações com melhora do VND
void info_inc_imp_iterations_vnd();

// Incrementa quantidade de iterações sem melhora do VND
void info_inc_noimp_iterations_vnd();

// Incrementa quantidade de vezes que realizou a perturbação de mover operador
void info_inc_move_perturbation();

// Incrementa quantidade de vezes que realizou a perturbação aleatória
void info_inc_random_perturbation();

// Incrementa quantidade de vezes que realizou a perturbação cross
void info_inc_cross_perturbation();

// Salva o tempo gasto na criação de uma rota, em milissegundos
void info_set_time_constructive(clock_t start, clock_t end);

// Salva o tempo gasto na viabilização das rotas, em milissegundos
void info_set_time_enables     (clock_t start, clock_t end);

// Salva o tempo gasto no algoritmo VND, em milissegundos
void info_set_time_vnd         (clock_t start, clock_t end);

// Salva o tempo gasto no algoritmo VNS, em milissegundos
void info_set_time_vns         (clock_t start, clock_t end);

// Salva o custo obtido com o algoritmo construtivo
void info_set_cost_constructive(int cost);

// Salva o custo obtido com o algoritmo viabilizador
void info_set_cost_enables     (int cost);

// Salva o custo obtido com o algoritmo VNS
void info_set_cost_vns         (int cost);

// Salva o custo obtido com uma iteração de melhora do VNS
void info_save_improvement_vns(int value);

// Salva as rotas obtidas
void info_set_routes(Route *r);

// Salva a seed utilizada na instância
void info_set_seed(int seed);

// Imprime no terminal as estatísticas obtidas com a execução
void info_print();

// Escreve as estatísticas obtidas em todas as execuções em um arquivo
void info_print_arr_file(Info **arr, int size);

// Escreve a melhor rota obtida em um arquivo
void info_print_solution_file(Info **arr, int size);

// Escreve a média das estatísticas de todas as execuções
void info_print_results_file(Info **arr, int size);

// Escreve as iterações em que houve melhora e o seu valor em um arquivo
void info_print_vectors(Vector *improv, Vector *it_improv, int seed, char *instance, int control);

// Escreve as estatísticas da melhor execução obtida em um arquivo, formatado em LaTeX
void info_print_table_result(Info **arr, int size);

// Escreve a média das estatísticas de todas as execuções em um arquivo, formatado em LaTeX
void info_print_table_infos (Info **arr, int size);

// Limpa todos os registros salvos na variável Info global
void info_reset();

// Libera a memória das rotas de uma estrutura info
void info_route_destroy();

// Libera memória de um array de Info
void info_arr_destroy(Info **arr, int size);

// Libera a memória de uma estrutura Info
void info_destroy();

// Imprime estatísticas de execução de uma instância, usada para escrita de relatórios e artigos
void info_print_ERI(Info **arr, int size);

#endif