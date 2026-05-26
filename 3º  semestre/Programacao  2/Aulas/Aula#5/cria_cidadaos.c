#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a struct
typedef struct {
    char id[11];
    int hacking;
    int criminalidade;
} cidadao;

int main()
{
    // Criando um array de cidadãos
    cidadao cidadaos_teste[] = 
    {
        {"V", 85, 8},
        {"Jackie", 70, 6},
        {"Judy", 60, 4},
        {"Panam", 90, 9},
        {"Johnny", 75, 5},
        {"Lucy", 65, 3}
    };

    // Calculando o número de cidadãos
    int num_cidadaos = sizeof(cidadaos_teste) / sizeof(cidadao);

    // Abrindo o arquivo para escrita binária
    FILE *arquivo = fopen("cyberpunk.bin", "wb");

    if (!arquivo)
    {
        printf("Erro ao criar o arquivo.\n");
        return 1;
    }

    // Escrevendo os dados dos cidadãos no arquivo
    fwrite(cidadaos_teste, sizeof(cidadao), num_cidadaos, arquivo);

    // Fechando o arquivo
    fclose(arquivo);

    printf("Arquivo 'cyberpunk.bin' criado com sucesso com %d cidadãos.\n", num_cidadaos);

    return 0;
}