/* 
dividindo o problema em blocos

1. Abrir arquivos com segurança
2. Ler o arquivo palavra por palavra
3. Armazenar e contar as palavras
4. Descobrir o vencedor e limpando a bagunça 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char nome_arquivo[256];

    FILE* arquivo;

    printf("Digite o nome do arquivo: ");
    scanf("%255s", nome_arquivo);

    arquivo = fopen(nome_arquivo, "r");

    if(arquivo == NULL)
    {
        printf("Erro: O Arquivo %s não pode ser aberto.\n", nome_arquivo);
        return 1;
    }

    printf("Arquivo %s aberto com sucesso.\n", nome_arquivo);




    fclose(arquivo);
    return 0;

}