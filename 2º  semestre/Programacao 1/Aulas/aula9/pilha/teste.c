// Tipo abstrato de dado "pilha de inteiros"
// Prof. Carlos Maziero - DINF/UFPR, Out 2024
//
// Este TAD implementa uma pilha de números inteiros com capacidade para
// até CAP valores, definida no momento de sua criação.
//
// Arquivo de teste.

#include <stdio.h>
#include "pilha.h"

#define TAMANHO 10

int main ()
{
  struct pilha_t *p ;
  int i, status, valor ;

  // cria uma pilha para até 10 elementos
  p = pilha_cria (TAMANHO) ;

  // empilha valores
  for (i = 0 ; i < TAMANHO + 2; i++)
  {
    valor = 10 * i ;
    printf ("Vou empilhar %d\n", valor) ;
    status = pilha_insere (p, valor) ;
    if (status < 0)
      printf ("Pilha cheia!\n") ;
    printf ("[ ") ;
    pilha_imprime (p) ;
    printf (" ]\n") ;
    pilha_topo (p, &valor) ;
    printf ("%d elementos, topo = %d\n\n", pilha_tamanho (p), valor) ;
  }

  // desempilha e imprime valores
  while (pilha_tamanho (p) > 0)
  {
    printf ("[ ") ;
    pilha_imprime (p) ;
    printf (" ]\n") ;
    pilha_topo (p, &valor) ;
    printf ("%d elementos, topo = %d\n", pilha_tamanho (p), valor) ;
    pilha_retira (p, &valor) ;
    printf ("Desempilhei %d\n\n", valor) ;
  }
  printf ("Pilha vazia!\n") ;

  // destroi a pilha
  p = pilha_destroi (p);
}

