#ifndef FILA
#define FILA

#include <stdio.h>
#include <stdlib.h>

// Nó da fila que guarda um ponteiro para um nó da arvore B
struct no_fila{
    struct nodo *nodo_arvore;
    struct no_fila *prox;
};

// Controle da fila
struct Fila{
    struct no_fila *inicio;
    struct no_fila *fim;
    int tamanho;
};

// função para imprimir mensagem de erro e encerrar execução 
void erro (char *msg);

// função que inicia a estrutura de fila
struct Fila* criarFila();

// insere os nodos da arvoreB na fila para impressão hierárquica
void inserir_fila(struct Fila *fila, struct nodo *nodo_atual);

// remove os nodos da fila para impressão hierárquica, retornando o nodo da arvoreB
struct nodo *remover_fila(struct Fila *fila);

// verifica se a fila esta vazia
bool fila_vazia(struct Fila *fila);

// função de limpeza da estrutura fila 
void liberar_fila(struct Fila *fila);

#endif

