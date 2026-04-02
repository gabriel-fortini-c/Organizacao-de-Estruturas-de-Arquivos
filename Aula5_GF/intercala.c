#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main (int argc, char ** argv)
{
    FILE *arqs[4], *saidaFinal;
	Endereco lista[4];
    int ativo[4] = {1, 1, 1, 1};
    int arquivosAbertos = 4;
    char nome[20];

    for (int i = 0; i < 4; i++) {
        sprintf(nome, "saida%d.dat", i + 1);
        arqs[i] = fopen(nome, "rb");
        if (arqs[i] == NULL) {
            fprintf(stderr, "Erro ao abrir %s\n", nome);
            return 1;
        }
        
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

        for (int i = 0; i < 4; i++) {
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
    
    for (int i = 0; i < 4; i++) {
    fclose(arqs[i]);
    }
    fclose(saidaFinal);

    return 0;
}