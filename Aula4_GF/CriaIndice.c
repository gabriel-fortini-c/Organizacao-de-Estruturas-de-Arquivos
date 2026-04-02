#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _IndiceEndereco IndiceEndereco;
typedef struct _Endereco Endereco;

struct _IndiceEndereco
{
    char cep[8];
    unsigned int posicao;
};

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
	return strncmp(((IndiceEndereco*)e1)->cep,((IndiceEndereco*)e2)->cep,8);
}

int main (int argc, char *argv[])
{
    FILE *f, *saida;
    Endereco e;
    long tamanho, numEndereco;

    f = fopen("cep_ordenado.dat", "rb");

    fseek(f, 0, SEEK_END);
    tamanho = ftell(f);
    printf("Tamanho: %ld | Registros: %ld\n", tamanho, numEndereco);
    numEndereco = tamanho/sizeof(Endereco);
    rewind(f);

    IndiceEndereco *indices = (IndiceEndereco*) malloc(numEndereco*sizeof(IndiceEndereco));

    int qt = fread(&e, sizeof(Endereco), 1, f);
    unsigned int i = 0;
    
    while (qt > 0) {
        strncpy(indices[i].cep, e.cep, 8);
        indices[i].posicao = i;

        qt = fread(&e, sizeof(Endereco), 1, f);
        i++;
    }

    qsort(indices, numEndereco, sizeof(IndiceEndereco), compara);

    saida = fopen("saida.dat", "wb");

    fwrite(indices, sizeof(IndiceEndereco), numEndereco, saida);

    fclose(f);
    fclose(saida);
    
    return 0;
}
