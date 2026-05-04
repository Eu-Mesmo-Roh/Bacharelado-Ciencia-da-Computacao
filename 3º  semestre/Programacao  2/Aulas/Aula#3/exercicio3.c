#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//aplicando o código padrão
int main()
{
    char str[256];
    int numeroColuna;

    printf("Digite uma linha no formato CSV:\n");

    fgets(str, sizeof(str), stdin);

    str[strcspn(str, "\n")] = 0;

    numeroColuna = 1;

    char *token = strtok(str, ",");

    while(token != NULL)
    {
        printf("coluna %d: %s\n", numeroColuna, token);

        numeroColuna++;

        token = strtok(NULL, ",");
    }

    return 0;
}


/* #include <stdio.h>
#include <stdlib.h> // Necessário para malloc e free
#include <string.h> // Necessário para strtok e strcspn

int main() {
    char *linha;
    int tamanho_maximo;

    // 1. Perguntamos o tamanho e alocamos dinamicamente
    printf("Digite o tamanho maximo da linha CSV que deseja inserir: ");
    scanf("%d", &tamanho_maximo);

    // TRUQUE IMPORTANTE: Limpar o buffer do teclado!
    // Isso consome o '\n' (Enter) que sobrou do scanf acima
    while (getchar() != '\n'); 

    // Alocamos espaço (+1 para o caractere nulo '\0' no final)
    linha = (char *)malloc((tamanho_maximo + 1) * sizeof(char));

    if (linha == NULL) {
        printf("Erro: Nao foi possivel alocar memoria.\n");
        return 1;
    }

    // 2. Lemos a string inteira, incluindo espaços
    printf("Digite a linha no formato CSV (ex: Nome,Idade,Cidade):\n");
    // Passamos o tamanho_maximo + 1 para o fgets saber o limite da memória
    fgets(linha, tamanho_maximo + 1, stdin);

    // Removemos o 'ENTER' (\n) do final da string capturado pelo fgets
    linha[strcspn(linha, "\n")] = 0;

    int numeroColuna = 1;

    // 3. Cortamos a string usando strtok
    char *token = strtok(linha, ",");

    while (token != NULL) {
        printf("Coluna %d: %s\n", numeroColuna, token);
        numeroColuna++;
        token = strtok(NULL, ",");
    }

    // 4. SEMPRE liberar a memória alocada no final
    free(linha);

    return 0;
} */