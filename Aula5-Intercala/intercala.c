#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define K 4

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int main (int argc, char ** argv)
{
    FILE *arqs[K], *saidaFinal;
	Endereco lista[K];
    int ativo[K];
    int arquivosAbertos = K;
    char nome[20];

    for (int i = 0; i < K; i++) {
        sprintf(nome, "saida%d.dat", i + 1);
        arqs[i] = fopen(nome, "rb");
        if (arqs[i] == NULL) {
            fprintf(stderr, "Erro ao abrir %s\n", nome);
            return 1;
        }
        
        ativo[i] = 1;

        if (fread(&lista[i], sizeof(Endereco), 1, arqs[i]) <= 0) {
            ativo[i] = 0;
            arquivosAbertos--;
        }
    }

    saidaFinal = fopen("saida-final.dat", "wb");
    if (saidaFinal == NULL) {
            fprintf(stderr, "Erro ao abrir saida-final.dat\n");
            return 1;
        }

    while (arquivosAbertos > 0) {

        int IndiceMenor = -1;

        for (int i = 0; i < K; i++) {
            if (ativo[i]) {
                if (IndiceMenor == -1 || strncmp(lista[i].cep, lista[IndiceMenor].cep, 8) < 0) {
                    IndiceMenor = i;
                }
            }
        } 

        fwrite(&lista[IndiceMenor], sizeof(Endereco), 1, saidaFinal);

        if (fread(&lista[IndiceMenor], sizeof(Endereco), 1, arqs[IndiceMenor]) == 0) {
            ativo[IndiceMenor] = 0;
            arquivosAbertos--;
        }
    }
    
    for (int i = 0; i < K; i++) {
        fclose(arqs[i]);
    }
    fclose(saidaFinal);

    return 0;
}