#include <stdio.h>
#include <stdlib.h>

struct no
{
    int chave;
    struct no *fe;
    struct no *fd;
    struct no *pai;
};

// faz a criação do novo nó
struct no *cria_no(int chave)
{
    struct no *novo = malloc (sizeof(struct no));
    
    if (!novo)
        return NULL;

    novo->chave = chave;
    novo->fe = NULL;
    novo->fd = NULL;
    novo->pai = NULL;

    return novo;
}

// usado para arvores sem ponteiro para o pai

/* 
struct no *binary(struct no *raiz, int chave)
{
    if (!raiz)
        return cria_no(chave);

    if (raiz->chave > chave)
        raiz->fe = binary(raiz->fe, chave);
    else if (raiz->chave < chave)
        raiz->fd = binary(raiz->fd, chave);
    
    return raiz;
}
*/

// Função auxiliar que faz a busca da posição correta para o novo nó
struct no *auxbinary(struct no *raiz, int chave, struct no *pai)
{
    if (!raiz)
    {
        struct no *novo = cria_no(chave);

        if (novo != NULL)
            novo->pai = pai;

        return novo;
    }
    
    if (raiz->chave > chave)
        raiz->fe = auxbinary(raiz->fe, chave, raiz);
    else if (raiz->chave < chave)
        raiz->fd = auxbinary(raiz->fd, chave, raiz);

    return raiz;    
}

// chama a função auxiliar para que a raiz tenha seu pai null
struct no *binary(struct no *raiz, int chave)
{
    return auxbinary(raiz, chave, NULL);
}

// imprime a arvore em ordem
void imprimirT(struct no *raiz)
{
    if (!raiz)
        return;
    
    imprimirT(raiz->fe);
    printf("%d\n\n", raiz->chave);
    imprimirT(raiz->fd);
}

// Libera a arvore com travessia pós ordem
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

            raiz = binary(raiz, chave);

            printf("*** Nodo cadastrado com sucesso ***\n\n");

            break;
        }

        case 2:
            if(!raiz)
            {
                printf("\n\n*** Arvore Vazia ***\n\n\n\n");
                break;
            }
            
            printf("** Imprimindo em Em-ordem **\n");
            imprimirT(raiz);
            break;

        case 0:
            printf("\n\n*** Encerrando o programa ***\n\n\n");
            break;

        default:
            printf("Erro - Entrada invalida");
            break;
        }
    } while (opcao != 0);

    freeT(raiz);

    return 0;
}