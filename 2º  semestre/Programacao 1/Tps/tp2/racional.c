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
long aleat(long min, long max)
{
  return min + rand() % (max - min + 1);
}

/* Máximo Divisor Comum entre a e b      */
/* calcula o MDC pelo método de Euclides */
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

/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc(long a, long b)
{
  return (a * b) / mdc(a, b);
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r(struct racional r)
{
  if (valido_r(r) == 0)
    return r; /* retorna a função sem modificação */

  long divisor = mdc(r.num, r.den); /* calcula o maximo divisor comum do num e den */

  r.num /= divisor;
  r.den /= divisor;

  if (r.den < 0)
  {
    r.num = -r.num;
    r.den = -r.den;
  }

  return r;
}

/* implemente as demais funções de racional.h aqui */

/* Cria um número racional com o numerador e denominador indicados. */
struct racional cria_r(long numerador, long denominador)
{
  struct racional novo; /* racional = tipo,  novo = nome da variavel */

  novo.num = numerador;
  novo.den = denominador;

  return novo;
}

/* Retorna 1 se o racional r for válido ou 0 se for inválido.
 * Um racional é inválido se seu denominador for zero */
int valido_r(struct racional r)
{
  if (r.den != 0)
    return 1;

  return 0;
}

/* Retorna um número racional aleatório na forma simplificada.
 * Deve ser sorteado o numerador e depois o denominador.
 * o racional gerado pode ser válido ou inválido.
 * O numerador e o denominador devem ser inteiros entre min e max. */
struct racional sorteia_r(long min, long max)
{
  struct racional fracao;

  long num, den;

  num = aleat(min, max);
  den = aleat(min, max);

  fracao = cria_r(num, den);

  if (valido_r(fracao) == 0)
    return fracao;

  return simplifica_r(fracao);
}

/* Imprime um racional r, respeitando estas regras:
   - o racional deve ser impresso na forma simplificada;
   - não imprima espaços em branco e não mude de linha;
   - a saída deve ter o formato "num/den", a menos dos casos abaixo:
     - se o racional for inválido, imprime a mensagem "NaN" (Not a Number);
     - se o numerador for 0, imprime somente "0";
     - se o denominador for 1, imprime somente o numerador;
     - se o numerador e denominador forem iguais, imprime somente "1";
     - se o racional for negativo, o sinal "-" vem antes do numerador;
     - se numerador e denominador forem negativos, o racional é positivo. */
void imprime_r(struct racional r)
{
  r = simplifica_r(r);

  if (valido_r(r) == 0)
    printf("NaN");

  else if (r.num == 0)
    printf("0");

  else if (r.den == 1)
    printf("%ld", r.num);

  else if (r.den == r.num)
    printf("1");

  else if (r.den < 0)
    printf("%ld/%ld", r.num * (-1), r.den * (-1));

  else
    printf("%ld/%ld", r.num, r.den);
}

/* Compara dois racionais r1 e r2. Retorno: -2 se r1 ou r2 for inválido,
 * -1 se r1 < r2, 0 se r1 = r2 ou 1 se r1 > r2 */
int compara_r(struct racional r1, struct racional r2)
{
  r1 = simplifica_r(r1); /*simplifica os numeros antes da operação*/
  r2 = simplifica_r(r2);

  /* faz a multiplicação cruzada */
  long resultado1 = r1.num * r2.den;
  long resultado2 = r2.num * r1.den;

  if (!valido_r(r1) || !valido_r(r2)) /* testa se r1 ou r2 é invalido */
    return -2;

  else if (resultado1 == resultado2) /* testa se r1 = r2 */
    return 0;

  else if (resultado1 < resultado2) /*testa se r1 < r2*/
    return -1;

  else /*se r1 > r2*/
    return 1;
}

/* Retorna a soma dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 * 0 se r1 ou r2 for inválido ou se *r3 for nulo */
int soma_r(struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  long multiplo;

  if (!valido_r(r1) || !valido_r(r2) || !r3) /*verifica se pelo menos um racionais for invalido ou o ponteiro for nulo retorna 0*/
    return 0;

  else if (r1.den != r2.den) /*realiza a soma quando o denominador dos racionais forem diferente */
  {
    multiplo = mmc(r1.den, r2.den);

    aux.num = (multiplo / r1.den * r1.num) + (multiplo / r2.den * r2.num);

    aux.den = multiplo;
  }

  else /*realiza a soma quando o denominado dos racionais forem iguais*/
  {
    aux.den = r1.den;
    aux.num = r1.num + r2.num;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

/* Retorna a subtracao dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 *         0 se r1 ou r2 for inválido ou se *r3 for nulo */
int subtrai_r(struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  long multiplo;

  if (!valido_r(r1) || !valido_r(r2) || !r3) /*verifica se pelo menos um racionais for invalido ou o ponteiro for nulo retorna 0*/
    return 0;

  else if (r1.den != r2.den) /*realiza a subtração quando o denominador dos racionais forem diferente */
  {
    multiplo = mmc(r1.den, r2.den);

    aux.num = (multiplo / r1.den * r1.num) - (multiplo / r2.den * r2.num);

    aux.den = multiplo;
  }

  else /*realiza a subtração quando o denominado dos racionais forem iguais*/
  {
    aux.den = r1.den;
    aux.num = r1.num - r2.num;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

/* Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 * 0 se r1 ou r2 for inválido ou se *r3 for nulo */
int multiplica_r(struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  if (!valido_r(r1) || !valido_r(r2) || !r3) /*verifica se pelo menos um racionais for invalido ou o ponteiro for nulo retorna 0*/
    return 0;

  else /*realiza a multiplicação dos racionais*/
  {
    aux.num = r1.num * r2.num;
    aux.den = r1.den * r2.den;
  }

  *r3 = simplifica_r(aux);

  return 1;
}

/* Retorna a divisao dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 * 0 se r1 ou r2 for inválido ou se *r3 for nulo */
int divide_r(struct racional r1, struct racional r2, struct racional *r3)
{
  struct racional aux;

  if (!valido_r(r1) || !valido_r(r2) || !r3) /*verifica se pelo menos um racionais for invalido ou o ponteiro for nulo retorna 0*/
    return 0;

  else if (r2.num == 0) /*se o numerador do r2 for 0 retorna 0*/
    return 0;

  else /*realiza a divisão dos racionais*/
  {
    aux.num = r1.num * r2.den;
    aux.den = r1.den * r2.num;
  }

  *r3 = simplifica_r(aux);

  return 1;
}