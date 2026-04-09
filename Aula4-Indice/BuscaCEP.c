#include <stdio.h>
#include <string.h>
#include <string.h>

typedef struct _Endereco Endereco;
typedef struct _IndiceEndereco IndiceEndereco;

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

struct _IndiceEndereco
{
    char cep[8];
    unsigned int posicao;
};

int main (int argc, char **argv)
{
    FILE *arqIndex, *arqCep;
    IndiceEndereco index;
    Endereco endEncontrado;
    long posEncontrada = -1;

    if (argc != 2) {
        fprintf(stderr, "Uso correto: ./%s [CEP desejado]\n", argv[0]);
        return 1;
    }

    arqIndex = fopen("saida.dat", "rb");

    if (arqIndex == NULL) { 
        fprintf(stderr, "Erro ao abrir o arquivo!\n");
        return 1;
    }

    fseek(arqIndex, 0, SEEK_END);
    long tamanho = ftell(arqIndex);
    long numEndereco = tamanho / sizeof(IndiceEndereco);

    long inicio = 0;
    long fim = numEndereco - 1;
    int contador = 0;

    while (inicio <= fim) {
        contador++;
        long meio = (inicio + fim) / 2;

        fseek(arqIndex, meio*sizeof(IndiceEndereco), SEEK_SET);
        fread (&index, sizeof(IndiceEndereco), 1, arqIndex);
        

        if (strncmp(index.cep, argv[1], 8) == 0) {
            posEncontrada = index.posicao;
            printf("Posição no arquivo de índices encontrada: %ld \n", posEncontrada);
            break;
        } else if (strncmp(argv[1], index.cep, 8) < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    if (posEncontrada == -1) {
        printf("CEP não encontrado no arquivo de indices.\n");
        return 0; 
    }

    arqCep = fopen("cep_ordenado.dat", "rb");
    fseek(arqCep, posEncontrada * sizeof(Endereco), SEEK_SET);
    fread(&endEncontrado, sizeof(Endereco), 1, arqCep);

    printf ("Total lido: %d \n", contador);
    printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",endEncontrado.logradouro,endEncontrado.bairro,endEncontrado.cidade,endEncontrado.uf,endEncontrado.sigla,endEncontrado.cep);

    fclose(arqIndex);
    fclose(arqCep);

    return 0;
}