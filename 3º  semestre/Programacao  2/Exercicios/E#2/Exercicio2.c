#include <stdio.h>
#include <stdlib.h>


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