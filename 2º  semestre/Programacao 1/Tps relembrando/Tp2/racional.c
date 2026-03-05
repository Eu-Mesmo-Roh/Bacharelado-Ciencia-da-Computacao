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

/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max)
{
  return min + rand() % (max - min + 1);
}

/* Máximo Divisor Comum entre a e b      */
/* calcula o MDC pelo método de Euclides */
long mdc (long a, long b)
{
  while (b != 0)
  {
    long temp = b;
    b = a % b;
    a = temp;
  }

  return a;
}

/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b)
{
  return (a * b) / mdc (a, b);
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r(struct racional r)
{
  if (r.den == 0)
    return r;
  
  long divisor;

  divisor = mdc (r.num, r.den);

  r.num /= divisor;
  r.den /= divisor;

  if(r.den < 0)
  {
    r.num = -r.num;
    r.den = -r.den;
  }

  return r;
}

/* implemente as demais funções de racional.h aqui */
struct racional cria_r (long numerador, long denominador)
{
  struct racional novo;

  novo.num = numerador;
  novo.den = denominador;

  return novo;
}

int valido_r (struct racional r)
{
  if(r.den == 0)
    return 0;
  
  return 1;
}

struct racional sorteia_r (long min, long max)
{
  struct racional fracao;

  long num, den;

  num = aleat (min, max);
  den = aleat (min, max);

  fracao = cria_r(num, den);

  if(valido_r(fracao) == 0)
    return fracao;

  return simplifica_r(fracao);
}

void imprime_r (struct racional r)
{
  r = simplifica_r(r);

  if(valido_r(r) == 0)
    printf("NaN");
  
  else if(r.num == 0)
    printf("0");
  
  else if(r.den == 1)
    printf("%ld", r.num);

  else if(r.num == r.den)
    printf("1");
  
  else if(r.num < 0)
    printf("%ld/%ld", r.num * (-1), r.den * (-1));
  
  else
    printf("%ld/%ld", r.num, r.den);
}

int compara_r (struct racional r1, struct racional r2)
{
  r1 = simplifica_r(r1);
  r2 = simplifica_r(r2);

  long resultado1 = r1.num * r2.den;
  long resultado2 = r2.num * r1.den;

  if(valido_r(r1) == 0 || valido_r(r2) == 0)
    return -2;
  
  else if(resultado1 < resultado2)
    return -1;
  
  else if(resultado1 == resultado2)
    return 0;
  
  else
    return 1;
}

int soma_r (struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  long multiplo;

  if(valido_r(r1) == 0 || valido_r(r2) == 0)
    return 0;
  
  else if (r1.den != r2.den)
  {
    multiplo = mmc(r1.den, r2.den);

    aux.num = ((multiplo / r1.den) * r1.num) + ((multiplo / r2.den) * r2.num);
    
    aux.den = multiplo;
  }

  else
  {
    aux.num = r1.num + r2.num;
    aux.den = r1.den;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

int subtrai_r (struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  long multiplo;

  if(!valido_r(r1) || !valido_r(r2) || !r3)
    return 0;

  else if (r1.den != r2.den)
  {
    multiplo = mmc(r1.den, r2.den);

    aux.num = ((multiplo / r1.den) * r1.num) - ((multiplo / r2.den) * r2.num);

    aux.den = multiplo;
  }

  else
  {
    aux.num = r1.num - r2.num;
    aux.den = r1.den;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

int multiplica_r (struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  if(!valido_r(r1) || !valido_r(r2) || !r3)
    return 0;

  else
  {
    aux.num = r1.num * r2.num;
    aux.den = r1.den * r2.den;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

int divide_r (struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  if (!valido_r(r1) || !valido_r(r2) || !r3 || r2.num == 0)
    return 0;

  else
  {
    aux.num = r1.num * r2.den;
    aux.den = r1.den * r2.num;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

