#include "graphviz_print.h"
#include "../adjacency_list/list.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"

void _system_call_graphviz(char *file_name){
    char call[ 24 + strlen(file_name) ];
    sprintf(call, "dot -Kneato -T%s %s -O &", TYPE, file_name);
    system(call);
}

void _vertex_file_write(void *vertices, int size, FILE *arq){
    Vector *v = vertices;
    char asp = '"';

    fprintf(arq, "node[fontcolor = white, fillcolor = black, style = filled, shape = circle, fontsize = %c12.5%c];\n", asp, asp);

    for(int i = 0; i < size; i++){
        Data *d = vector_get(v, i);
        fprintf(arq, "v%d [pos = %c%.2f, %.2f!%c];\n", i, asp, (float)data_return_x(d)/3, (float)data_return_y(d)/3, asp);
    }
}

void _graph_file_write(Graph *g, int size, FILE *arq, int direction){
    char edge[3] = "-";
    (direction == DIRECTED) ? strcat(edge, ">") : strcat(edge, "-");

    if( MATRIX ){
        matrix_file_write(graph_return_adjacencies(g), size, arq, edge);

    } else if( LIST ){
        list_file_write(graph_return_adjacencies(g), size, arq, edge);

    }

}

void _route_file_write(int *route, int size, FILE *arq){
    char asp = '"';
    
    for( int i = 0; i < size; i++ )
        fprintf(arq, "v%d -> v%d [color = %cgreen%c];\n", route[i], route[i+1], asp, asp);
}

void img_print_vertex(Graph *g, char *file_name){

    FILE *arq_vertex = fopen(file_name, "w");
    char asp = '"';

    if( !arq_vertex ){
        printf("Não foi possivel abrir o arquivo %s\n", file_name);
        return;
    }

    fprintf(arq_vertex, "graph {\n");

    _vertex_file_write(graph_return_vertex_vector(g), graph_return_num_vertex(g), arq_vertex);

    fprintf(arq_vertex, "}");
    fclose(arq_vertex);

    _system_call_graphviz(file_name);
}

void img_print_graph(Graph *g, char *file_name){

    FILE *arq_graph = fopen(file_name, "w");
    char asp = '"';
    char open_arq[11] = "di", suf[9] = "graph {\n";
    int direction = graph_return_direction(g);

    if( !arq_graph ){
        printf("Não foi possivel abrir o arquivo %s\n", file_name);
        return;
    }

    if( direction == DIRECTED ){
        strcat(open_arq, suf);
    } else {
        // direction == UNDIRECTED
        strcpy(open_arq, suf);
    }

    fprintf(arq_graph, "%s", open_arq);

    _vertex_file_write(graph_return_vertex_vector(g), graph_return_num_vertex(g), arq_graph);
    _graph_file_write(g, graph_return_num_vertex(g), arq_graph, direction);
    if( graph_has_route(g) ) _route_file_write(graph_return_route(g), graph_return_num_vertex(g) - 1, arq_graph);

    fprintf(arq_graph, "}");
    fclose(arq_graph);

    _system_call_graphviz(file_name);
}