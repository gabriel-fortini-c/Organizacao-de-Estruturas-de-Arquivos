#include <stdio.h>
#include <stdlib.h>
#include "CSVParser.h"
#include <string.h>

// Total casos e mortes América do Sul - Covid

#define READ_BUF_SIZE 8192

void callback(char** cols, int ncols, void* userData) {
    int *counts = (int*) userData;    
    // printf("%s %s %s \n", cols[1], cols[4], cols[7]);
    
    // cols[1] -> continente
    // cols[4] -> número de casos
    // cols[7] -> número de mortes
    if (strncmp(cols[1], "South America", 13) == 0) {
        // counts[0] -> total linhas  
        // counts[1] -> total casos
        // counts[2] -> total mortes
        counts[1] += atoi(cols[5]);
        counts[2] += atoi(cols[8]);
    }

    (counts[0])++;
}

int main() {
    // qtds[0] -> total linhas  
    // qtds[1] -> total casos
    // qtds[2] -> total mortes
    int qtds[3] = {0};
    int qt;
    unsigned int total = 0;
    char* buf = (char*) malloc(READ_BUF_SIZE);

    CSVParser csv;
    CSVParser_init(&csv);
    FILE *f = fopen("./owid-covid-data.csv","rb");
    
    if(f) {
        qt = fread(buf,1,READ_BUF_SIZE,f);
        while(qt > 0) {
            total += qt;
            CSVParser_processLines(&csv,buf,qt,callback,qtds);
            qt = fread(buf,1,READ_BUF_SIZE,f);
        }
        fclose(f);
        CSVParser_processLines(&csv,"\n",1,callback,qtds);
        printf("Total rows: %d\n", qtds[0]);
        printf("Total Casos: %d\n", qtds[1]);
        printf("Total Mortes: %d\n", qtds[2]);
    }
    
    free(buf);
    return 0;
}