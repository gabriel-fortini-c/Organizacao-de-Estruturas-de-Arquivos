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

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main (int argc, char **argv)
{
    FILE *arq;
    Endereco *e;
    long numRegistros, tamanhoArquivo;

    arq = fopen("cep.dat", "rb");
    if (arq == NULL) { // Boa prática: checar se o arquivo abriu
        fprintf(stderr, "Erro ao abrir o arquivo!\n");
        return 1;
    }

    fseek(arq, 0, SEEK_END);
    tamanhoArquivo = ftell(arq);
    numRegistros = tamanhoArquivo / sizeof(Endereco);
    rewind(arq);

    int tamanhoNormal = numRegistros / K;
    int resto = numRegistros % K;
    e = (Endereco*) malloc(sizeof(Endereco)*(tamanhoNormal + resto));

    for (int i = 0; i < K; i++) {     
        int qtdAtual;
        if (i != (K - 1)) {
            fread(e,sizeof(Endereco),tamanhoNormal,arq);
            qsort(e, tamanhoNormal, sizeof(Endereco), compara);
            qtdAtual = tamanhoNormal;
        } else {
            fread(e,sizeof(Endereco),(tamanhoNormal) + resto,arq);
            qsort(e, (tamanhoNormal + resto), sizeof(Endereco), compara);
            qtdAtual = tamanhoNormal + resto;
        }

        char nomeArq[20];
        snprintf(nomeArq, sizeof(nomeArq), "saida%d.dat", i + 1);
        FILE *a = fopen(nomeArq, "wb");

        fwrite(e, sizeof(Endereco), qtdAtual, a);

        fclose(a);
    }

    free(e);
    fclose(arq);

    return 0;
}