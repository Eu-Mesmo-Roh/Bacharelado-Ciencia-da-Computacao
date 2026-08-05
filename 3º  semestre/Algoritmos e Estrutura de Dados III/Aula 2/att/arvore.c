#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

pessoa* criar_pessoa(char nome[25], int idade, long CPF)
{
    pessoa *nova_pessoa;

    nova_pessoa = (pessoa*) malloc(sizeof(pessoa));

    if(!nova_pessoa)
    {
        printf("Erro ao criar o cadastro");
        return NULL;
    }

    strncpy(nova_pessoa->nome, nome, 24);
    nova_pessoa->nome[24] =  '\0';
    nova_pessoa->idade = idade;
    nova_pessoa->chave = CPF;

    nova_pessoa->esq = NULL;
    nova_pessoa->dir = NULL;

    return nova_pessoa;
}

pessoa* inserir_pessoa(pessoa *raiz, pessoa *nova_pessoa)
{
    if(!raiz)
    {
        raiz = nova_pessoa;
        return raiz;
    }

    if(!raiz->esq)
    {
        raiz->esq = nova_pessoa;
        return raiz;
    }
    else if(!raiz->dir)
    {
        raiz->dir = nova_pessoa;
        return raiz;
    }

    else
        inserir_pessoa(raiz->esq, nova_pessoa);
    
    return raiz;
}