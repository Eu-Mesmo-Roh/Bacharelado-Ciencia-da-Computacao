/*
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 16/09/2024 para a disciplina CI1001 - Programação 1.
 */

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* programa principal */
int main()
{
  int n, max, i;

  struct racional fracao1;
  struct racional fracao2;

  srand(0);

  do
  {
    scanf("%d", &n);
  } while (n < 0 || n > 100); // limita de 0 a 100

  do
  {
    scanf("%d", &max);
  } while (max < 0 || max > 30); // limita de 0 a 100

  i = 1;
  while (i <= n)
  {
    printf("%d: ", i);

    fracao1 = sorteia_r(max * (-1), max);
    fracao2 = sorteia_r(max * (-1), max);

    imprime_r(fracao1);
    printf(" ");
    imprime_r(fracao2);
    printf(" ");

    if (!valido_r(fracao1) || !valido_r(fracao2))
    {
      printf("NUMERO INVALIDO\n");
      printf(" ");
      return 1;
    }

    if (!valido_r(divide_r(fracao1, fracao2)))
    {
      printf("DIVISAO INVALIDA\n");
      printf(" ");
      return 1;
    }

    imprime_r(soma_r(fracao1, fracao2));
    printf(" ");
    imprime_r(subtrai_r(fracao1, fracao2));
    printf(" ");
    imprime_r(multiplica_r(fracao1, fracao2));
    printf(" ");
    imprime_r(divide_r(fracao1, fracao2));

    printf(" \n");
    i++;
  }

  return 0;
}
