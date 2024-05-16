#include "graphviz_print.h"
#include "../adjacency_matrix/matrix.h"
#include "../Vector/vector.h"
#include "../graph/graph.h"
#include <dirent.h>

enum Color {
    BLACK,
    GREEN,
    BLUE,
    YELLOW,
    RED,
    PURPLE,
    ORANGE,
    BROWN,
    PINK,
    CYAN,
    GRAY
};

/* =============================================== FUNÇÕES INTERNAS ================================================================== */

void _directory_imgs_verify(){
    DIR *dir = opendir("imgs/");
    if( dir ){
        closedir( dir );
    } else {
        system("mkdir imgs/");
    }
}

// Cria a imagem <filename>.png de acordo com os comandos neato do graphviz
void _system_call_graphviz(char *file_name){
    char call[ 24 + strlen(file_name) ];
    sprintf(call, "neato -T%s %s -O &", TYPE, file_name);
    system(call);
}

// Escreve os vértices do grafo no arquivo
void _vertex_file_write(void *vertices, int size, FILE *arq){
    Vector *v = vertices;
    char asp = '"';
    
    fprintf(arq, "node[fontcolor = white, fillcolor = black, style = filled, shape = circle, fontsize = %c10%c, overlap = %cfalse%c];\n", asp, asp, asp, asp);

    if (!vector_size(v)) return;

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

// Imprime as arestas do grafo no arquivo
void _graph_file_write(Graph *g, int size, FILE *arq, int direction){
    char edge[3] = "-";
    (direction == DIRECTED) ? strcat(edge, ">") : strcat(edge, "-");
    matrix_file_write(graph_return_adjacencies(g), size, arq, edge);
}

// Retorna a i-ésima cor, de acordo com o enum Color
char *_color_return(int i){
    switch( i ){
        case BLACK: return "black";
        case GREEN: return "green";
        case BLUE: return "blue";
        case YELLOW: return "yellow";
        case RED: return "red";
        case PURPLE: return "purple";
        case ORANGE: return "orange";
        case BROWN: return "brown";
        case PINK: return "pink";
        case CYAN: return "cyan";
        default : return "gray";
    }
}

// Escreve as rotas no arquivo arq
void _route_file_write(Graph *g, int size, FILE *arq){
    char asp = '"', ini_color[16], end_color[5];

    sprintf(ini_color, "edge [color = %c", asp);
    sprintf(end_color, "%c];\n", asp);

    fprintf(arq, "edge [dir = forward];\n");

    for(int i = 0; i < size; i++){
        char color[30] = "";
        strcat(color, ini_color);
        strcat(color, _color_return(i));
        strcat(color, end_color);
        fprintf(arq, "\n%s\n", color);

        Route *r = graph_return_route(g);
        int route_size = route_return_size(r, i), *rt = route_return_route(r, i);
        for( int j = 0; j < route_size; j++ ){
            fprintf(arq, "v%d ", rt[j]);
                if( j < route_size - 1) fprintf(arq, "-- ");
            
        }
        fprintf(arq, ";\n");
    }
    
}

/* =================================================================================================================================== */

void img_print_vertex(Graph *g, char *fileName){

    _directory_imgs_verify();
    char file_name[200] = "imgs/";
    strcat(file_name, fileName);
    
    FILE *arq_vertex = fopen(file_name, "w");

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

void img_print_graph(Graph *g, char *fileName){

    _directory_imgs_verify();
    char file_name[200] = "imgs/";
    strcat(file_name, fileName);

    FILE *arq_graph = fopen(file_name, "w");
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

    fprintf(arq_graph, "}");
    fclose(arq_graph);

    _system_call_graphviz(file_name);
}

void img_print_route(Graph *g, char *fileName){
    
    _directory_imgs_verify();
    char file_name[200] = "imgs/";
    strcat(file_name, fileName);

    FILE *arq_graph = fopen(file_name, "w");
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
    _route_file_write(g, graph_return_trucks(g), arq_graph);

    fprintf(arq_graph, "}");
    fclose(arq_graph);

    _system_call_graphviz(file_name);
}

void img_print_graph_per_edge(Graph *g1, Graph *g2, int it, char *fileName){

    _directory_imgs_verify();
    char file_name[200] = "imgs/";
    strcat(file_name, fileName);

    char real_filename[206];
    sprintf(real_filename, "%s%d.dot",file_name, it);

    FILE *arq_graph = fopen(real_filename, "w");
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

    fprintf(arq_graph, "edge [ color = black, len = 5 ];\n");

    _graph_file_write(g2, graph_return_num_vertex(g2), arq_graph, graph_return_direction(g2));

    fprintf(arq_graph, "}");
    fclose(arq_graph);

    _system_call_graphviz(real_filename);

}