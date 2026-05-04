#include <stdio.h>
#include <stdlib.h>


int main()
{
    FILE* arquivo;

    arquivo = fopen("escrito.txt", "w");

    if(arquivo == NULL)
    {
        printf("erro ao abrir o arquivo");
        exit(1);
    }

    fprintf(arquivo, "Escrevendo no novo arquivo em C.\n");
    fputs("adicionando outra linha com fputs.\n", arquivo);

    fclose(arquivo);

    printf("arquivo criado e escrito com sucesso");

    return 0;
}