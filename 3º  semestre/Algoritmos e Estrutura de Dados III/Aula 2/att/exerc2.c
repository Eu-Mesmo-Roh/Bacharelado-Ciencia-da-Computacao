#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "travessia.h"

void main()
{
    int opcao;
    
    printf("Bem vindo a minha arvore de cadastro de pessoas\n\n");

    printf("*** Opções ***\n");
    printf("1 - Cadastrar Pessoa\n");
    printf("2 - Localizar Pessoa\n");
    printf("3 - Excluir pessoa\n\n");

    scanf("%d", &opcao);

    switch (opcao)
    {
        case 1:

            break;

        case 2:

            break;

        case 3:

            break;
        
        default:
            printf("Erro - Entrada invalida");
            break;
    }
}