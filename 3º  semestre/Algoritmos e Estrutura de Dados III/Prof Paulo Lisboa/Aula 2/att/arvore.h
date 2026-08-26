#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct pessoa
{
    char nome[25];
    int idade;
    long chave;
    struct pessoa *esq;
    struct pessoa *dir;
}pessoa;

// cria o nó da pessoa
pessoa* criar_pessoa(char nome[25], int idade, long CPF);

// Insere a pessoa na arvore
pessoa* inserir_pessoa(pessoa *raiz, pessoa *nova_pessoa);

// Calculando a altura da arvore
int calcularAltura(pessoa *raiz);

// Liberando memória
void liberar_arvore(pessoa *raiz);

#endif