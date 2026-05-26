#include <stdio.h>
#include <stdlib.h>

// Definindo a struct
typedef struct {
    char id[11];
    int hacking;
    int criminalidade;
} cidadao;

int main()
{
    int cidadaos_totais, soma_hacking, criminalidade_alta;
    float media_hacking;

    FILE *arquivo;
    cidadao atual;

    // Abrindo o arquivo para leitura binária
    arquivo = fopen("cyberpunk.bin", "rb");

    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    cidadaos_totais = 0;
    soma_hacking = 0;
    criminalidade_alta = 0;

    printf("\n****Lista de Cidadãos****\n");

    // Lendo os dados do arquivo e processando
    while (fread(&atual, sizeof(cidadao), 1, arquivo) == 1)
    {
        // Exibindo os dados do cidadão
        printf("ID: %s, Hacking: %d, Criminalidade: %d\n", atual.id, atual.hacking, atual.criminalidade);

        // Atualizando as estatisticas
        cidadaos_totais++;
        soma_hacking += atual.hacking;

        if (atual.criminalidade > 5)
            criminalidade_alta++;
    }

    // Fechando o arquivo
    fclose(arquivo);

    // Exibindo as estatísticas
    printf("\n****Estatísticas****\n");
    printf("\nTotal de Cidadãos: %d\n", cidadaos_totais);

    if (cidadaos_totais > 0)
    {
        media_hacking = (float)soma_hacking / cidadaos_totais;
        printf("Médias das habilidades de hacking: %.2f\n", media_hacking);
    }
    else
        printf("Nenhum cidadão encontrado para calcular a média de hacking.\n");

    printf("Número de cidadãos com criminalidade acima de 5: %d\n", criminalidade_alta);

    return 0;
}