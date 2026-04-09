#include <stdio.h>
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
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

int main(int argc, char**argv)
{
    FILE *arq;
	Endereco e;
	int qt;
	int contador = 0;

    if (argc != 2) {
        fprintf(stderr, "Uso correto: ./%s [CEP desejado]\n", argv[0]);
        return 1;
    }

    arq = fopen("", "rb");
    if (arq == NULL) { // Boa prática: checar se o arquivo abriu
        fprintf(stderr, "Erro ao abrir o arquivo!\n");
        return 1;
    }

    printf("Tamanho da estrutura que armazena endereço: %ld \n\n", sizeof(Endereco));

    fseek(arq, 0, SEEK_END);
    long tamanhoArquivo = ftell(arq);
    long numRegistros = tamanhoArquivo / sizeof(Endereco); 
    long inicio = 0;
    long fim = numRegistros - 1;

    while (inicio <= fim) {
        contador++;
        long meio = (inicio + fim) / 2;
        fseek(arq, meio*sizeof(Endereco), SEEK_SET);
        qt = fread (&e, sizeof(Endereco), 1, arq);


        if (strncmp(argv[1], e.cep, 8) == 0) {
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
            break;
        } else if (strncmp(argv[1], e.cep, 8) < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    printf ("Total lido: %d", contador);
    fclose(arq);

    return 0;
}