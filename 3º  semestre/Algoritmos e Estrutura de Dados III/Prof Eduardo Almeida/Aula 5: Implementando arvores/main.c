#include <stdio.h>
#include <stdlib.h>

struct no
{
    int chave;
    struct no *fe;
    struct no *fd;
    struct no *pai;
};

struct no *cria_no(int chave)
{
    struct no *n = malloc (sizeof(struct no));
    
    if (!n)
        return NULL;

    n->chave = chave;
    n->fe = NULL;
    n->fd = NULL;
    n->pai = NULL;

    return n;
}

struct no *binary(struct no *raiz, int chave)
{
    if (!raiz)
    {
        return cria_no(chave);
    }

    if (raiz->chave > chave)
        raiz->fe = binary(raiz->fe, chave);
    else if (raiz->chave < chave)
        raiz->fd = binary(raiz->fd, chave);
    
    return raiz;
}

void imprimirT(struct no *raiz)
{
    if (!raiz)
        return;
    
    imprimirT(raiz->fe);
    printf("%d", raiz->chave);
    imprimirT(raiz->fd);
}

void freeT(struct no *raiz)
{
    if (raiz != NULL)
    {
        freeT(raiz->fe);
        freeT(raiz->fd);
        free(raiz);
    }

}

int main()
{
    //Criando nossa arvore generica
    struct no *raiz;
    int opcao;

    raiz = NULL;

    printf("Bem vindo a minha arvore genérica\n\n");

    do
    {
        printf("*** Opções ***\n");
        printf("1 - Cadastrar nodo\n");
        printf("2 - Percorrer a arvore em em-ordem\n");
        printf("0 - Sair\n\n");

        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
        {
            int chave;

            printf("\nDigite a chave: ");
            scanf("%d", &chave);

            struct no *novo_no = cria_no(chave);


            raiz = binary(novo_no, chave);

            printf("*** Pessoa Cadastrada com sucesso ***\n\n");

            break;
        }

        case 2:
            if(!raiz)
            {
                printf("\n\n*** Arvore Vazia ***\n\n\n\n");
                break;
            }
            
            printf("** Imprimindo em Em-ordem **\n");
            em_ordem(raiz);
            break;

        case 0:
            printf("\n\n*** Encerrando o programa ***\n\n\n");
            break;

        default:
            printf("Erro - Entrada invalida");
            break;
        }
    } while (opcao != 0);




    return 0;
}