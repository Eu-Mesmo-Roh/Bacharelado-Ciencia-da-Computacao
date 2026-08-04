#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char nome[25];
    int idade;
    long int chave;
    pessoa *esq;
    pessoa *dir;
}pessoa;

#endif