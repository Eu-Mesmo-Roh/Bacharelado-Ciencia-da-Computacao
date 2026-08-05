#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "travessia.h"

int main()
{
    int opcao;
    pessoa *raiz;

    raiz = NULL;

    printf("Bem vindo a minha arvore de cadastro de pessoas\n\n");

    do
    {
        printf("*** Opções ***\n");
        printf("1 - Cadastrar Pessoa\n");
        printf("2 - Percorrer a arvore em pré-ordem\n");
        printf("3 - Percorrer a arvore em em-ordem\n");
        printf("4 - Percorrer a arvore em pós-ordem\n");
        printf("5 - Calcular a altura da arvore\n");
        printf("0 - Sair\n\n");

        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            char nome[25];
            int idade;
            long CPF;

            printf("Digite o Nome da Pessoa: ");
            scanf(" %24[^\n]", nome);

            printf("\nDigite a Idade da Pessoa: ");
            scanf("%d", &idade);

            printf("\nDigite o CPF da Pessoa: ");
            scanf("%ld", &CPF);

            pessoa *nova_pessoa = criar_pessoa(nome, idade, CPF);

            raiz = inserir_pessoa(raiz, nova_pessoa);

            printf("*** Pessoa Cadastrada com sucesso ***\n\n");

            break;
        }

        case 2:
            if (!raiz)
            {
                printf("\n\n*** Arvore Vazia ***\n\n\n\n");
                break;
            }

            printf("** Imprimindo em Pré-ordem **\n");
            pre_ordem(raiz);
            break;

        case 3:
            if(!raiz)
            {
                printf("\n\n*** Arvore Vazia ***\n\n\n\n");
                break;
            }
            
            printf("** Imprimindo em Em-ordem **\n");
            em_ordem(raiz);
            break;

        case 4:
            if(!raiz)
            {
                printf("\n\n*** Arvore Vazia ***\n\n\n\n");
                break;
            }

            printf("** Imprimindo em Pós-ordem **\n");
            pos_ordem(raiz);
            break;
        
        case 5:
        {
            int alt;

            alt = calcularAltura(raiz);
            printf("A altura da arvore é: %d\n\n\n", alt);
            break;
        }

        case 0:
            printf("\n\n*** Encerrando o programa ***\n\n\n");
            break;

        default:
            printf("Erro - Entrada invalida");
            break;
        }
    } while (opcao != 0);

    liberar_arvore(raiz);

    return 0;
}