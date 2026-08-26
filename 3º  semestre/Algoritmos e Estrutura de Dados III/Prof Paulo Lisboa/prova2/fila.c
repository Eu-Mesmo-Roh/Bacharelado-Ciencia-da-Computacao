#include "fila.h"

void erro (char *msg){
    fprintf (stderr, "ERRO: %s\n", msg);
    exit (1);
}

struct Fila* criarFila(){
    struct Fila *fila = malloc(sizeof(struct Fila));

    if (!fila)
        erro("Falha ao alocar memoria para fila");

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;

    return fila;
}

void inserir_fila(struct Fila *fila, struct nodo *nodo_atual){
    if(!fila || !nodo_atual)
        erro("Fila ou nodo nulo");

    struct no_fila *novo_no_fila = malloc(sizeof(struct no_fila));

    if (!novo_no_fila)
        erro("Falha ao alocar memoria para nó da fila");

    novo_no_fila->nodo_arvore = nodo_atual;
    novo_no_fila->prox = NULL;

    if (fila->fim)
        fila->fim->prox = novo_no_fila;
    else
        fila->inicio = novo_no_fila;

    fila->fim = novo_no_fila;
    fila->tamanho++;
}

struct nodo *remover_fila(struct Fila *fila){
    if(!fila || !fila->inicio)
        erro("Fila vazia");

    struct no_fila *aux = fila->inicio;
    struct nodo *info = aux->nodo_arvore;

    fila->inicio = aux->prox;
    if (!fila->inicio)
        fila->fim = NULL;

    free(aux);
    fila->tamanho--;

    return info;
}

bool fila_vazia(struct Fila *fila){
    return fila->tamanho == 0;
}

void liberar_fila(struct Fila *fila){
    struct no_fila *aux, *proxi;
    if(!fila)
        return;

    aux = fila->inicio;
    while (aux != NULL){
        proxi = aux->prox;
        free (aux);
        aux = proxi;
    }

    free(fila);
}