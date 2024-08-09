#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main ( int argc, char* argv[] ) {

    if(argc < 2) exit(printf("Error missing cvrp file\n"));
    FILE *arq = fopen(argv[1], "r");
    if(!arq) exit(printf("Eror fopen failed.\n"));

    char buffer[100] = "hi";
    while (strcmp(buffer, "DEMEND_SECTION\n"))
    {
        // printf("%s\n", buffer);
        fgets(buffer, sizeof(buffer), arq);
    }

    // printf("\nString Controle\n");

    fgets(buffer, sizeof(buffer), arq);
    int sum = 0, qtd = 0;
    while(strcmp(buffer, "EDGE_WEIGHT_SECTION\n")){
        // printf("%s\n", buffer);
        int demand = 0;
        sscanf(buffer, "%*d %d", &demand);
        sum += demand;
        qtd++;
        fgets(buffer, sizeof(buffer), arq);
    }

    printf("Demanda total: %d\nNós: %d\n\n", sum, qtd);

    int capTrucks = ceil( sum / 0.76 );
    int numTrucks = ceil( qtd / 7.5 );
    int capacity = ceil( capTrucks / numTrucks );

    printf("Full capacity: %d\n", capTrucks);
    printf("Trucks: %d\n", numTrucks);
    printf("Capacity: %d\n", capacity);

    return 0;
}