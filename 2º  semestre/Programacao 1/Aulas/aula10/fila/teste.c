// Tipo abstrato de dado "fila de inteiros"
// Prof. Carlos Maziero - DINF/UFPR, Out 2024
//
// Este TAD implementa uma fila de números inteiros com capacidade para
// até CAP valores entre 0 e CAP-1, definida no momento de sua criação.
//
// Arquivo de teste.

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define TAMANHO 5

int main ()
{
  struct fila_t *f ;
  int i, status, valor ;

  // cria uma fila para até TAMANHO elementos
  f = fila_cria (TAMANHO) ;

  // insere valores
  printf ("Inserções:\n") ;
  for (i = 0 ; i < TAMANHO + 2; i++)
  {
    valor = 10 * i ;
    printf ("Vou inserir %d\n", valor) ;
    status = fila_insere (f, valor) ;
    if (status < 0)
      printf ("Fila cheia!\n") ;

    printf ("[ ") ;
    fila_imprime (f) ;
    printf (" ]\n") ;

    fila_primeiro (f, &valor) ;
    printf ("%d elementos, primeiro = %d\n\n", fila_tamanho (f), valor) ;
  }

  // retira/insere valores
  printf ("Remoções e inserções alternadas:\n") ;
  printf ("[ ") ;
  fila_imprime (f) ;
  printf (" ]\n") ;

  for (i = 0 ; i < TAMANHO; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      fila_retira (f, &valor) ;
      printf ("Retirei %d\n", valor) ;
    }
    printf ("[ ") ;
    fila_imprime (f) ;
    printf (" ]\n") ;

    for (int j = 0; j < 4; j++)
    {
      valor = 10 * i + j ;
      printf ("Vou inserir %d\n", valor) ;
      status = fila_insere (f, valor) ;
      if (status < 0)
        printf ("Fila cheia!\n") ;
    }
    printf ("[ ") ;
    fila_imprime (f) ;
    printf (" ]\n\n") ;
  }

  // esvazia e imprime valores
  printf ("Esvaziando:\n") ;
  printf ("[ ") ;
  fila_imprime (f) ;
  printf (" ]\n") ;

  while (fila_tamanho (f) > 0)
  {
    fila_retira (f, &valor) ;
    printf ("Retirei %d\n", valor) ;
    printf ("[ ") ;
    fila_imprime (f) ;
    printf (" ]\n") ;
  }
  printf ("Fila vazia!\n") ;

  // destroi a fila
  f = fila_destroi (f);
}

