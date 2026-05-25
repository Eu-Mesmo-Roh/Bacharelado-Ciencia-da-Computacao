#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct nodo 
{
    char dado;
    struct nodo *ant;
    struct nodo *prox;
};

struct deque 
{
    struct nodo *inicio;
    struct nodo *fim;
};

// Insere o nodo no inicio do deque
void insereInicio(struct deque *fipi, struct nodo *novo) 
{
    if (fipi->inicio == NULL) 
    {
        fipi->inicio = novo;
        fipi->fim = novo;
        novo->prox = NULL;
        novo->ant = NULL;
    } 
    else 
    {
        novo->prox = fipi->inicio;
        novo->ant = NULL;
        fipi->inicio->ant = novo;
        fipi->inicio = novo;
    }
}

// Insere no fim do deque
void insereFim(struct deque *fipi, struct nodo *novo)
{
    if(fipi->inicio == NULL)
    {
        fipi->inicio = novo;
        fipi->fim = novo;
        novo->ant = NULL;
        novo->prox = NULL;
    }
    else
    {
        novo->ant = fipi->fim;
        novo->prox = NULL;
        fipi->fim->prox = novo;
        fipi->fim = novo;
    }
}

// Remove o nodo do inicio do deque
struct nodo *removeInicio(struct deque *fipi)
{
    if(fipi->inicio == NULL)
        return NULL;
    
    struct nodo *aux;

    aux = fipi->inicio;

    fipi->inicio = fipi->inicio->prox;

    if(fipi->inicio != NULL)
        fipi->inicio->ant = NULL;

    else
        fipi->fim = NULL;
    
        aux->ant = NULL;
        aux->prox = NULL;

    return aux;
}

// Remove o nodo do final do deque
struct nodo *removeFim(struct deque *fipi)
{
    if(fipi->fim == NULL)
        return NULL;
    
    struct nodo *aux;
    
    aux = fipi->fim;

    fipi->fim = fipi->fim->ant;

    if(fipi->fim != NULL)
        fipi->fim->prox = NULL;
    
    else
        fipi->inicio = NULL;

        aux->ant = NULL;
        aux->prox = NULL;
    
    return aux;
}

int main()
{
    struct deque meuDeque;
    meuDeque.inicio = NULL;
    meuDeque.fim = NULL;

    char *palavra;
    int tam_max;
    int ehPalindromo = 1;

    printf("Digite o tamanho máximo da palavra que deseja verificar;\n");
    scanf("%d", &tam_max);

    palavra = (char *)malloc((tam_max + 1) * sizeof(char));

    if (palavra == NULL) 
    {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    printf("Digite  a palavra desejada: ");
    scanf("%s", palavra);

    for (int i = 0; i < strlen(palavra); i++)
    {
        struct nodo *novo = (struct nodo *)malloc(sizeof(struct nodo));

        if (novo == NULL) 
        {
            printf("Erro ao alocar memória.\n");
            return 1;
        }

        novo->dado = palavra[i];
        insereFim(&meuDeque, novo);
    }

    while (meuDeque.inicio != NULL && meuDeque.inicio != meuDeque.fim)
    {
        struct nodo *inicio = removeInicio(&meuDeque);
        struct nodo *fim = removeFim(&meuDeque);

        if (inicio->dado != fim->dado)
        {
            ehPalindromo = 0;
            free(inicio);
            free(fim);
            break;
        }

        free(inicio);
        free(fim);
    }

    while (meuDeque.inicio != NULL)
    {
        struct nodo *sobra = removeInicio(&meuDeque);
        free(sobra);
    }

        if (ehPalindromo)
            printf("\nA palavra '%s' É um palindromo.\n", palavra);
        else
            printf("\nA palavra '%s' NÃO é um palindromo.\n", palavra);

    free(palavra);

    return 0;
}
