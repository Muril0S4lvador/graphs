#include "graph.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"
#include "../algorithms/algorithms.h"

struct Info{
    int total_iterations;
    int imp_iterations;
    double time_ms;
    int srand_seed;
    char *instance;
    Route *routes;
    int optimal;
};
