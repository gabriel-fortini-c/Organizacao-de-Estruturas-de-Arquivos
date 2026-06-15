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

int main (int argc, char **argv)
{
    FILE *arq, *saida1, *saida2;
    Endereco *e;
    long posicao, qtd, metade;

    arq = fopen("cep.dat", "rw");
    
    fseek(arq, 0, SEEK_END);
    posicao = ftell(arq);
    qtd = posicao/sizeof(Endereco);
    
    e = (Endereco*) malloc(sizeof(Endereco)*qtd);
    rewind(arq);
    if(fread(e,sizeof(Endereco),qtd,arq) == qtd)
	{
		printf("Todos lidos = OK\n");
	}
    qsort(e, qtd, sizeof(Endereco), compara);
    printf("Ordenado = OK\n");

    saida1 = fopen("cepOrd_1.dat", "wb");
    fwrite(e, sizeof(Endereco), (qtd / 2), saida1);

    saida2 = fopen("cepOrd_2.dat", "wb");
    fwrite(e, sizeof(Endereco), (qtd / 2) + 1, saida2);

    fclose(arq);
    fclose(saida1);
    fclose(saida2);
    free(e);

    return 0;
}