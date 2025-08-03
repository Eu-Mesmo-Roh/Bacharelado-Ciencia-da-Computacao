// Tipo abstrato de dado "pilha de inteiros"
// Prof. Carlos Maziero - DINF/UFPR, Out 2024
//
// Este TAD implementa uma pilha de números inteiros com capacidade
// para até CAP elementos, definida no momento de sua criação.
//
// Implementação com vetor.

#ifndef PILHA
#define PILHA

// estrutura interna de uma pilha
struct pilha_t
{
  // a completar
} ;

// Cria uma pilha vazia com a capacidade informada e a retorna;
// Retorna NULL em caso de erro
struct pilha_t *pilha_cria (int capacidade);

// Remove todos os elementos da pilha, libera a memória e retorna NULL
struct pilha_t *pilha_destroi (struct pilha_t *pilha);

// Insere o elemento no topo da pilha (politica LIFO);
// Retorna o número de elementos na pilha após a operação
// ou -1 em caso de erro
int pilha_insere (struct pilha_t *pilha, int elem);

// Retira o elemento do topo da pilha (politica LIFO) e o devolve;
// Retorna o número de elementos na pilha após a operação
// ou -1 em caso de erro
int pilha_retira (struct pilha_t *pilha, int *elem);

// devolve o elemento no topo da pilha, sem removê-lo da pilha
// Retorna o número de elementos na pilha ou -1 em caso de erro
int pilha_topo (struct pilha_t *pilha, int *elem);

// Retorna o tamanho da pilha (número de elementos na pilha)
// ou -1 em caso de erro
int pilha_tamanho (struct pilha_t *pilha);

// Retorna a capacidade da pilha (número de elementos que ela aceita)
// ou -1 em caso de erro
int pilha_capacidade (struct pilha_t *pilha);

// Imprime o conteúdo da pilha do topo à base, no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois e sem \n no fim
void pilha_imprime (struct pilha_t *pilha);

#endif
