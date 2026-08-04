#ifndef TRAVESSIA_H
#define TRAVESSIA_H

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"


void pre_order(pessoa *raiz)
{
    if(!raiz)
        return;
    
    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("CPF: %ls\n\n", raiz->chave);

    pre_order(raiz->esq);
    pre_order(raiz->dir);
}

void em_ordem(pessoa *raiz)
{
    if(!raiz)
        return;
    
    em_ordem(raiz->esq);

    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("CPF: %ls\n\n", raiz->chave);

    em_ordem(raiz->dir);
}

void pos_order(pessoa *raiz)
{
    if(!raiz)
        return;
    
    pos_order(raiz->esq);

    pos_order(raiz->dir);

    printf("Nome: %s\n", raiz->nome);
    printf("Idade: %d\n", raiz->idade);
    printf("CPF: %ls\n\n", raiz->chave);
}

#endif