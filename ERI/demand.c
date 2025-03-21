#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEN 7.5
#define APERTO 0.92

char nome[100] = "";
char comment[100] = "";

typedef struct {
    int id, demand;
    double x, y;
} Node;


void write(Node *instance, int dimension, float totalDemand, FILE *f);

/**
 * Recebe uma instância de um algoritmo de cobertura de vértices
 * e ajusta os parâmetros para que fique mais parecido com as
 * encontradas na literatura
 */
int main ( int argc, char* argv[] ) {

    if(argc < 2) exit(printf("Error missing cvrp file\n"));
    FILE *arq = fopen(argv[1], "r");
    if(!arq) exit(printf("Eror fopen failed.\n"));

    char buffer[1000];
    int dimension = 0, capacity = 0;
    fgets(buffer, sizeof(buffer), arq); // NAME
    char name[50];
    sscanf(buffer, "%*[^:]: %s", name);
    fgets(buffer, sizeof(buffer), arq); // COMMENT
    char *token = strtok(buffer, "()"); token = strtok(NULL, ")");
    token[strlen(token) - 1] = (token[strlen(token) - 1] == '\n') ? '\0' : token[strlen(token) - 1]; // tira \n caso precise
    strcpy(comment, token);
    fgets(buffer, sizeof(buffer), arq); // TYPE
    fgets(buffer, sizeof(buffer), arq); // DIMENSION
    sscanf(buffer, "%*[^:]: %d", &dimension);
    
    Node *instance = malloc(sizeof(Node) * dimension);
    double m[dimension][3];

    while (strcmp(buffer, "NODE_COORD_SECTION\n")) // NODE_COORD_SECTION
    {
        fgets(buffer, sizeof(buffer), arq);
    }

    for(int i = 0; i < dimension; i++){
        fgets(buffer, sizeof(buffer), arq);
        sscanf(buffer, "%*d %lf %lf", &m[i][0], &m[i][1]);
    }
    fgets(buffer, sizeof(buffer), arq); // DEMEND_SECTION
    int totalDemand = 0;
    for(int i = 0; i < dimension; i++){
        
        fgets(buffer, sizeof(buffer), arq); 
        sscanf(buffer, "%*d %lf", &m[i][2]);
        
        instance[i].x = m[i][0];
        instance[i].y = m[i][1];
        instance[i].demand = m[i][2];

        totalDemand += m[i][2];
    }
    
    fgets(buffer, sizeof(buffer), arq); // EDGE_WEIGHT_SECTION

    write(instance, dimension, totalDemand, arq);

    return 0;
}

void write(Node *instance, int dimension, float totalDemand, FILE *f) {
    int qtdCaminhao = ceil(dimension / DEN);
    int caminhaoCap = ceil(totalDemand / APERTO);
    int demanda = ceil(caminhaoCap / qtdCaminhao);

    sprintf(nome, "C-n%d-k%d.vrp", dimension, qtdCaminhao);

    FILE *arq = fopen(nome, "w");

    fprintf(arq, "NAME : %s\n", nome);
    fprintf(arq, "COMMENT : (%s)\n", comment);
    fprintf(arq, "TYPE : CVRP\n");
    fprintf(arq, "DIMENSION : %d\n", dimension);
    fprintf(arq, "DIMENSION : LOWER_ROW_VIX\n");
    fprintf(arq, "CAPACITY : %d\n", demanda);
    fprintf(arq, "NODE_COORD_SECTION\n");
    
    for(int i = 0; i < dimension; i++) {
        fprintf(arq, "%d %.3lf %.3lf\n", i + 1, instance[i].x, instance[i].y);
    }
    
    fprintf(arq, "DEMAND_SECTION\n");
    
    for(int i = 0; i < dimension; i++) {
        fprintf(arq, "%d %d\n", i + 1, instance[i].demand);
    }
    fprintf(arq, "EDGE_WEIGHT_SECTION\n");

    char buffer[1000] = "";
    while (!feof(f))
    {
        fgets(buffer, sizeof(buffer),f);
        fputs(buffer, arq);
    }
    
    fclose(arq);
    fclose(f);
}

/*
pontos de coleta
- Demanda do ponto é a soma de todas as residências
- Cada residência adiciona 10 na demanda
- 1 residência não pode ser contabilizada em 2 pontos de coleta

Capacidade Total
- Capacidade total dos caminhões é Demanda Total / 0.85

Caminhões
- Número de caminhões é número de vértices / 7.5  arredondado pra cima

Capacidade de cada caminhão
- Capacidade total / qtd caminhões
*/