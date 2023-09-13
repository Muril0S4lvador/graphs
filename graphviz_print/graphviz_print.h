#ifndef _GRAPHVIZ_PRINT_H_
#define _GRAPHVIZ_PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../graph.h"
#include "../data/data.h"
#include "../Vector/vector.h"

#define TYPE "png"

void img_print_vertex(Graph *g, char *file_name);

void img_print_graph(Graph *g, char *file_name);

#endif