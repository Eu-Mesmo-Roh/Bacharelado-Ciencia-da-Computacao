#ifndef ARVORE_B_H_
#define ARVORE_B_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

// estrutura de um nodo para arvorB
struct nodo {
    int32_t n_chaves; // numero de chaves em um nodo 
    int32_t *chaves; // chaves do nodo 
    struct nodo **filhos; // ponteiro para array dos filhos de cada nodo
    bool eh_folha; // booleano que informa se o nodo é folha ou não é 
};

// estrutura da arvoreB
struct arvoreB {
  struct nodo *raiz; // ponteiro para um nodo raiz
  int32_t t_arvore; // grau minimo da arvore 
};

// função para alocação de um nodo, retorna o nodo em caso de sucesso
// ou mensagem de erro em caso de falha
struct nodo *cria_nodo (int32_t t_arvore, bool eh_folha);

// função para alocação de uma arvoreB seguindo especificações Cormen
// retorna a arvore em caso de sucesso ou mensagem de erro
struct arvoreB* criarArvoreB(int32_t t_arvore);

// função auxiliar para inserção 
void dividir_filho (struct nodo* nodo, int32_t indice, int32_t t_arvore);

// função auxiliar para inserção
struct nodo *dividir_Raiz (struct arvoreB* arvore);

// função auxiliar para inserção
void inserir_não_cheio (struct nodo* atual, int32_t chave, int32_t t_arvore);

// função para inserir um novo nodo numa arvore b
// insere na raiz, caso so ela exista e, se não, na insere em uma folha 
void inserirArvoreB (struct arvoreB* arvore, int32_t chave);

// imprime a arvoreB de forma hierárquica, mostrando os níveis da arvore
void imprimirArvoreB (struct arvoreB* arvore);

// imprime a arvore em ordem crescente.
void imprimirEmOrdem (struct arvoreB* arvore);

//faz a impressão em profundidade usando travessia em ordem
void travessiaEmOrdem (struct nodo* atual);

// função auxiliar para realizar a busca de forma recursiva
struct nodo* buscarArvoreBrec (struct nodo* atual, int32_t chave, 
                            int32_t* idxEncontrado);

struct nodo* buscarArvoreB (struct arvoreB* arvore, int32_t chave,
                            int32_t* idxEncontrado);

// função que faz a travessia pos-ordem para liberar a memoria alocada para os nodos e chaves da arvore
void liberarNodos(struct nodo* atual);

// função que libera a memória alocada para a arvoreB, seus nodos e chaves
void deletarArvore (struct arvoreB* arvore);

#endif