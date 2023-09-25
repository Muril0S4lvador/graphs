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

    fprintf(arq, "node[fontcolor = white, fillcolor = black, style = filled, shape = circle, fontsize = %c10%c, overlap = %cfalse%c];\n", asp, asp, asp, asp);

    if(!vector_size(v)) return;

    Data *d = vector_get(v, 0);
    int denX = data_return_x(d), denY = data_return_y(d);
    for(int i = 1; i < size; i++){
        d = vector_get(v, i);
        int dataX = data_return_x(d), dataY = data_return_y(d);
        denX = ( dataX > denX ) ? dataX : denX;
        denY = ( dataY > denY ) ? dataY : denY;
    }

    for(int i = 0; i < size; i++){
        d = vector_get(v, i);
        fprintf(arq, "v%d [pos = %c%.2f, %.2f!%c];\n", i, asp, (float)data_return_x(d)/ (denX * 0.1) , (float)data_return_y(d)/ (denY * 0.1), asp);
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
        fprintf(arq, "v%d -> v%d [color = %cred%c];\n", route[i], route[i+1], asp, asp);
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

void img_print_graph_per_edge(Graph *g1, Graph *g2, int it, char *file_name){

    char real_filename[50];
    sprintf(real_filename, "%s%d.dot",file_name, it);

    FILE *arq_graph = fopen(real_filename, "w");
    char asp = '"';
    char open_arq[9] = "graph {\n";

    if( !arq_graph ){
        printf("Não foi possivel abrir o arquivo %s\n", real_filename);
        return;
    }

    if( graph_return_direction(g2) == DIRECTED ){
        printf("Necessária uma mst não direcionada para função\n");
        return;
    }

    fprintf(arq_graph, "%s", open_arq);

    _vertex_file_write(graph_return_vertex_vector(g1), graph_return_num_vertex(g1), arq_graph);

    fprintf(arq_graph, "edge [ color = black ];");

    _graph_file_write(g2, graph_return_num_vertex(g2), arq_graph, graph_return_direction(g2));

    fprintf(arq_graph, "}");
    fclose(arq_graph);

    _system_call_graphviz(real_filename);

}