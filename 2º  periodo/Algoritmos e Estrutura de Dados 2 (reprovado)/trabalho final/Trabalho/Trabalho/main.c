#include "ordena.h"

int main() 
{
    const char *arquivo_entrada = "entrada.txt";  // Nome do arquivo de entrada com os números
    const char *arquivo_saida = "saida.txt";   // Nome do arquivo de saída final

    gerar_runs(arquivo_entrada);
    printf("Geração das runs finalizada.\n");

    int num_runs = contar_runs();
    printf("Número de runs geradas: %d\n", num_runs);
    
    intercala_runs_em_fases(num_runs, arquivo_saida);
    printf("Intercalação finalizada. Resultado salvo em %s.\n", arquivo_saida);

    return 0;
}
