/*
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
 */

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/*
 * Implemente aqui as funcoes definidas no racionais.h; caso precise,
 * pode definir aqui funcoes auxiliares adicionais, que devem ser usadas
 * somente neste arquivo.
 */

/* Maximo Divisor Comum entre a e b      */
/* calcula o mdc pelo metodo de Euclides */
long mdc(long a, long b)
{
  while (b != 0)
  {
    long temp = b;
    b = a % b;
    a = temp;
  }

  return a;
}

/* Minimo Multiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc(long a, long b)
{
  return (a * b) / mdc(a, b);
}

/* Simplifica o número racional indicado no parâmetro.
 * Por exemplo, se o número for 10/8 muda para 5/4.
 * Retorna 1 em sucesso e 0 se r for inválido ou o ponteiro for nulo.
 * Se ambos numerador e denominador forem negativos, o resultado é positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador. */
int simplifica_r(struct racional *r)
{
  if (valido_r(r) == 0)
    return 0; /* retorna a função sem modificação */

  long divisor = mdc(r->num, r->den); /* calcula o maximo divisor comum do num e den */

  r->num /= divisor;
  r->den /= divisor;

  if (r->den < 0)
  {
    r->num = -r->num;
    r->den = -r->den;
  }

  return 1;
}

/* implemente as demais funções de racional.h aqui */

struct racional *cria_r(long numerador, long denominador)
{
  struct racional *novo = malloc(sizeof(struct racional));

  if (novo == NULL)
    return NULL;

  novo->num = numerador;
  novo->den = denominador;

  return novo;
}

void destroi_r (struct racional *r)
{
  if (r != NULL)
    free(r);
}

int valido_r (struct racional *r)
{
  if (r == NULL)
    return 0;


  if (r->den == 0)
    return 0;

  return 1;
}

void imprime_r (struct racional *r)
{
  simplifica_r(r);

  if (r == NULL)
    printf("NULL");

  else if (valido_r(r) == 0)
    printf("NaN");

  else if (r->num == 0)
    printf("0");

  else if (r->den == 1)
    printf("%ld", r->num);

  else if (r->den == r->num)
    printf("1");

  else if (r->den < 0)
    printf("%ld/%ld", r->num * (-1), r->den * (-1));

  else
    printf("%ld/%ld", r->num, r->den);

}

int compara_r (struct racional *r1, struct racional *r2)
{
  long resultado1, resultado2;
  simplifica_r(r1);
  simplifica_r(r2);

  resultado1 = r1->num * r2->den;
  resultado2 = r2->num * r1->den;

  if (!valido_r(r1) || !valido_r(r2))
    return -2;

  else if (resultado1 == resultado2)
    return 0;

  else if (resultado1 < resultado2)
    return -1;

  else
    return 1;

}

int soma_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
  struct racional aux;
  long multiplo;

  if (!valido_r(r1) || !valido_r(r2) || !r1 || !r2 ||!r3)
    return 0;

  if (r1->den != r2->den) /*realiza a soma quando o denominador dos racionais forem diferente */
  {
    multiplo = mmc(r1->den, r2->den);

    aux.num = (multiplo / r1->den * r1->num) + (multiplo / r2->den * r2->num);

    aux.den = multiplo;
  }

  else /*realiza a soma quando o denominado dos racionais forem iguais*/
  {
    aux.den = r1->den;
    aux.num = r1->num + r2->num;
  }

  simplifica_r(&aux);

  *r3 = aux;

  return 1;

}

int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
  struct racional aux;
  long multiplo;

  if (!valido_r(r1) || !valido_r(r2) || !r1 || !r2 ||!r3)
    return 0;

  else if (r1->den != r2->den)
  {
    multiplo = mmc(r1->den, r2->den);

    aux.num = (multiplo / r1->den * r1->num) - (multiplo / r2->den * r2->num);

    aux.den = multiplo;
  }

  else
  {
    aux.den = r1->den;
    aux.num = r1->num - r2->num;
  }

  simplifica_r(&aux);
  *r3 = aux;

  return 1;

}

int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
    struct racional aux;

  if (!valido_r(r1) || !valido_r(r2) || !r1 || !r2 ||!r3)
    return 0;

  else
  {
    aux.num = r1->num * r2->num;
    aux.den = r1->den * r2->den;
  }

  simplifica_r(&aux);
  *r3 = aux;


  return 1;

}

int divide_r (struct racional *r1, struct racional *r2, struct racional *r3)
{
  struct racional aux;

  if (!valido_r(r1) || !valido_r(r2) || !r1 || !r2 ||!r3)
    return 0;

  else if (r2->num == 0)
    return 0;

  else
  {
    aux.num = r1->num * r2->den;
    aux.den = r1->den * r2->num;
  }

  simplifica_r(&aux);
  *r3 = aux;

  return 1;

}