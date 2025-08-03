// Tipo abstrato de dado "fila de inteiros"
// Prof. Carlos Maziero - DINF/UFPR, Out 2024
//
// Este TAD implementa uma fila de números inteiros com capacidade
// para até CAP elementos, definida no momento de sua criação.
//
// Implementação com vetores (completar).

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila_t *fila_cria (int capacidade)
{
    struct fila_t* fila;

    fila = malloc (sizeof(struct fila_t));
    if (fila == NULL)
        return NULL;

    fila->data = malloc (capacidade*sizeof(int));
    if (fila->data == NULL)
    {
        free(fila);
        return NULL;
    }

    fila->max = capacidade;
    fila->ultimo = 0;

    return fila;
}

struct fila_t *fila_destroi (struct fila_t *fila)
{

}

int fila_insere (struct fila_t *fila, int pielem)
{
    if (fila == NULL || fila->ultimo >= fila->max)
        return -1;
    
    fila->data[];

}

int fila_retira (struct fila_t *fila, int *pielem)
{
    int i;

    if (fila == NULL || fila->ultimo ==0)
        return -1;

    fila->ultimo--;
    *pielem = fila->data[0];
    for (i = 0; i < fila->ultimo; i++)
        fila->data[i] = fila->data[i+1];
    
    return fila->ultimo;

}

int fila_primeiro (struct fila_t *fila, int *pielem)
{
if (fila == NULL || fila->ultimo == 0)
    return -1;

*pielem = fila->data[0];

return fila->ultimo;

}

int fila_tamanho (struct fila_t *fila)
{
    if (fila == NULL)
        return -1;

    return  ; 
}

int fila_capacidade (struct fila_t *fila)
{
    if (fila == NULL)
        return -1;
    
    return fila->max;    

}

void fila_imprime (struct fila_t *fila)
{


}