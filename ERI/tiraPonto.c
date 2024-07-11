#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[] ){

    if(argc <= 1) exit(printf("Missing .txt file\n"));

    FILE *arq = fopen(argv[1], "r");
    FILE *write = fopen("eriTable.csv", "w");

    char buffer[500];
    while( !feof(arq) ){
        fgets(buffer, sizeof(buffer), arq);
        for(int i = 0; i < strlen(buffer); i++){
            if(buffer[i] == '.') buffer[i] = ',';
        }
        fputs(buffer, write);
    }


    return 0;
}