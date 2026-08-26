#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "travessia.h"

void pre_ordem(pessoa *raiz)
{
    if(!raiz)
        return;
        
    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("CPF: %ld\n\n", raiz->chave);

    pre_ordem(raiz->esq);
    pre_ordem(raiz->dir);
}

void em_ordem(pessoa *raiz)
{
    if(!raiz)
        return;

    
    em_ordem(raiz->esq);

    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("CPF: %ld\n\n", raiz->chave);

    em_ordem(raiz->dir);
}

void pos_ordem(pessoa *raiz)
{
    if(!raiz)
        return;
    
    pos_ordem(raiz->esq);

    pos_ordem(raiz->dir);

    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("CPF: %ld\n\n", raiz->chave);
}