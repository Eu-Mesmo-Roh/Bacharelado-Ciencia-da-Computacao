// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct item_t *item_cria(int valor)
{
    struct item_t *item;

    item = malloc(sizeof(struct item_t));
    if (item == NULL)
        return NULL;

    item->ant = NULL;
    item->prox = NULL;
    item->valor = valor;

    return item;
}

void item_destroi(struct item_t *item)
{
    if (item == NULL)
        return;
    else
        free(item);

    item = NULL;
}

struct lista_t *lista_cria()
{
    struct lista_t *lista;
    lista = malloc(sizeof(struct lista_t));

    if (lista == NULL)
        return NULL;

    lista->prim = NULL;
    lista->ult = NULL;
    lista->tamanho = 0;

    return lista;
}

struct lista_t *lista_destroi(struct lista_t *lst)
{
    struct item_t *atual, *prox;

    if (lst == NULL)
        return NULL;

    atual = lst->prim;

    while (atual != NULL)
    {
        prox = atual->prox;
        item_destroi(atual);
        atual = prox;
    }

    free(lst);

    lst = NULL;

    return NULL;
}

int lista_insere(struct lista_t *lst, int item, int pos)
{
    struct item_t *novo_item, *aux;

    if (lst == NULL)
        return -1;

    if ((novo_item = item_cria(item)) == NULL)
        return -1;

    if (lst->tamanho == 0)
    {
        lst->prim = novo_item;
        lst->ult = novo_item;
    }

    else if (pos > lst->tamanho || pos < 0)
    {
        novo_item->ant = lst->ult;
        lst->ult->prox = novo_item;
        lst->ult = novo_item;
    }

    else if (pos == 0)
    {
        novo_item->prox = lst->prim;
        lst->prim->ant = novo_item;
        lst->prim = novo_item;
    }

    else
    {
        aux = lst->prim;
        while (aux->prox != NULL && pos != 0)
        {
            aux = aux->prox;
            pos--;
        }

        novo_item->prox = aux;
        novo_item->ant = aux->ant;

        aux->ant->prox = novo_item;
        aux->ant = novo_item;
    }
    lst->tamanho++;

    return lst->tamanho;
}

struct item_t *item_poteiro_pos(struct lista_t *lst, int pos)
{
    struct item_t *aux;

    if (!lst)
        return NULL;

    /*percorre lista e decrementa posição até achar elemento*/
    aux = lst->prim;
    while (aux != NULL && pos != 0)
    {
        aux = aux->prox;
        pos--;
    }

    /*retorna NULL caso não encontre*/
    return aux;
}

int lista_retira(struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;

    if (lst == NULL || item == NULL)
        return -1;

    if (lst->tamanho == 0 || pos > lst->tamanho)
        return -1;
    else if (pos == 0)
    {
        aux = lst->prim;
        if (lst->tamanho == 1)
        {
            lst->prim = NULL;
            lst->ult = NULL;
        }
        else
        {
            lst->prim = aux->prox;
            aux->prox->ant = NULL;
        }
    }
    else if (pos < 0 || pos == lst->tamanho)
    {
        aux = lst->ult;
        lst->ult = aux->ant;
        aux->ant->prox = NULL;
    }
    else
    {
        aux = lst->prim;
        while (aux->prox != NULL && pos != 0)
        {
            aux = aux->prox;
            pos--;
        }

        aux->ant->prox = aux->prox;
        aux->prox->ant = aux->ant;
    }

    *item = aux->valor;
    item_destroi(aux);
    lst->tamanho--;

    return lst->tamanho;
}

int lista_consulta(struct lista_t *lst, int *item, int pos)
{
    struct item_t *aux;

    if (lst == NULL || item == NULL)
        return -1;

    aux = lst->prim;
    if (pos < 0)
        aux = lst->ult;
    else if (pos > lst->tamanho - 1)
        return -1;
    else
    {
        while (aux != NULL && pos != 0)
        {
            aux = aux->prox;
            pos--;
        }
    }

    *item = aux->valor;

    return lst->tamanho;
}

int lista_procura(struct lista_t *lst, int valor)
{
    struct item_t *aux;
    int cont;

    if (lst == NULL)
        return -1;

    aux = lst->prim;
    cont = 0;

    while (aux != NULL && valor != aux->valor)
    {
        aux = aux->prox;
        cont++;
    }

    if (aux == NULL)
        return -1;

    return cont;
}

int lista_tamanho(struct lista_t *lst)
{
    if (lst == NULL)
        return -1;

    return lst->tamanho;
}

void lista_imprime(struct lista_t *lst)
{
    struct item_t *aux;

    if (lst == NULL)
        return;

    aux = lst->prim;

    while (aux != NULL)
    {
        printf("%d", aux->valor);
        aux = aux->prox;

        if (aux != NULL)
            printf(" ");
    }
}