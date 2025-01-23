#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "src/graph/graph.h"
#include <time.h>

#define MAX_PATH 1024

typedef struct Instancias {
    float average_tightness;
    float average_demand;
    float average_nodes;
    float average_trucks;
    int size_group;
} Instancias;

Instancias* inicializa(int size){
    Instancias *i = calloc(size, sizeof(Instancias));
    return i;
}

Instancias fazMedia(Instancias i){
    i.average_tightness /= (float)i.size_group;
    i.average_demand /= (float)i.size_group;
    i.average_nodes /= (float)i.size_group;
    i.average_trucks /= (float)i.size_group;
    return i;
}

// Função para listar arquivos em um diretório
Instancias list_files(char *base_path, Instancias instancia) {
    struct dirent *dp;
    char real_base_path[64];
    sprintf(real_base_path, "entradas/%s", base_path);
    DIR *dir = opendir(real_base_path);
    printf("Lendo diretório: %s\n", real_base_path);

    if (!dir) {
        perror("opendir");
        exit(printf("Erro em repositorio %s", real_base_path));
    }

    while ((dp = readdir(dir)) != NULL) {
        // Ignora "." e ".."
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        // Constrói o caminho completo do arquivo ou diretório
        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/%s", real_base_path, dp->d_name);

        // Verifica se é um diretório
        if (dp->d_type == DT_DIR) {
            printf("Diretório: %s\n", path);
            // Chama a função recursivamente para listar arquivos no subdiretório
            // list_files(path);
        } else if (dp->d_type == DT_REG) {
            // Caso seja um arquivo regular
            printf("Arquivo: %s\n", path);
            Graph *g = graph_read_file_CVRPLIB(path);
            if( !g ) exit(printf("Erro ao ler grafo %s.", path));

            instancia.average_demand += graph_return_total_demand(g);
            instancia.average_nodes += graph_return_num_vertex(g);
            instancia.average_tightness += (float)graph_return_total_demand(g) / (graph_return_capacity(g) * graph_return_trucks(g));
            instancia.average_trucks += graph_return_trucks(g);
            if(!strcmp(path, "entradas/a/A-n33-k6.vrp")) printf("\n%.2f %.2f %.2f %.2f\n\n", instancia.average_demand, instancia.average_nodes, instancia.average_tightness, instancia.average_trucks);
            instancia.size_group++;
            
        }
    }

    closedir(dir);

    return instancia;
}

int main() {
    // Diretórios a serem lidos
    char *dirs[] = { "a", "p", "b", "e", "m", "f", "vix" };
    size_t num_dirs = sizeof(dirs) / sizeof(dirs[0]);

    Instancias* instancias = inicializa((int)num_dirs);

    for (size_t i = 0; i < num_dirs; ++i) {
        instancias[i] = list_files(dirs[i], instancias[i]);
        instancias[i] = fazMedia(instancias[i]);
        
    }

    printf("\n\n");
    printf("tightness - demand - nodes - trucks\n");
    for (size_t i = 0; i < num_dirs; ++i) {
        // printf("%s(%d): %.2f - %.2f - %.2f - %.2f\n", dirs[i], instancias[i].size_group, instancias[i].average_tightness, instancias[i].average_demand, instancias[i].average_nodes, instancias[i].average_trucks);
        printf("%s & %d & %.2f & %.2f & %.2f & %.2f\n", dirs[i], instancias[i].size_group, instancias[i].average_tightness, instancias[i].average_demand, instancias[i].average_nodes, instancias[i].average_trucks);
    }
    

    return 0;
}
