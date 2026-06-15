#include "ArvoreB.h"
#include <stdio.h>

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

int main (int argc, char **argv)
{
    FILE *saida, *arvore, *ceps;
    saida = fopen("intersecaoCEPs.dat", "wb");
    ArvoreB* a = ArvoreB_Abre("arvore.dat");
    ceps = fopen("8-10-cep-1.dat", "rb");

    if (!saida) {
        fprintf(stderr, "Não foi possível abrir o arquivo de saída."); 
        return 1;
    }
    if (!ceps) {
        fprintf(stderr, "Não foi possível abrir o arquivo de CEP."); 
        return 1;
    }

    Endereco e;
    int contador_lidos = 0;
    int contador_intersecao = 0;

    while(fread(&e, sizeof(Endereco), 1, ceps) > 0) {
        char chave[TAM_CHAVE+1];
        sprintf(chave, "%.8s", e.cep);
        
        if (ArvoreB_Busca(a, chave) != -1) {
            fwrite(&e, sizeof(Endereco), 1, saida);
            contador_intersecao++;
        }

        contador_lidos++;
    }

    printf("\nProcesso concluido!\n");
    printf("Total de intersecoes gravadas: %d\n", contador_intersecao);

    ArvoreB_Fecha(a);
    fclose(ceps);
    fclose (saida);

    return 0;
}