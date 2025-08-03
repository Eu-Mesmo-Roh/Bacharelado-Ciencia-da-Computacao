/*
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
 */

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* coloque aqui as funções auxiliares que precisar neste arquivo */

/* função que imprime o vetor na tela */
void imprimeVet(struct racional vet[], long tam)
{
    int i;

    if (!vet)
        return;

    printf("VETOR = ");

    for (i = 0; i < tam; i++)
    {
        imprime_r(vet[i]);
        printf(" ");
    }

    printf("\n");
}

/* função que retira os elemento NaN do vetor */
int retiraNaN(struct racional vet[], long tam)
{
    int i;

    for (i = 0; i < tam; i++)
    {
        /* verifica se o elemento do vetor na posição v[i] é valido*/
        if (!valido_r(vet[i]))
        {
            /* verifica se o elemento na ultima posição do vetor é valido
             * caso seja o remove */
            while (!valido_r(vet[tam - 1]))
                tam--;

            /* remove o elemento da posição v[i] */
            if (i != tam)
            {
                vet[i] = vet[tam - 1];
                tam--;
            }
        }
    }

    return tam;
}

/* função que mescla as subpartes do vetor que ja foi ordenada*/
void merge(struct racional vet[], int esquerda, int meio, int direita)
{
    /* variaveis que contem o tamanho das subpartes que serão mescladas
     * n1 = esquerda, n2 = direita */
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    /* sublistas temporarias sendo elas esquerda e direita */
    struct racional E[n1], D[n2];

    /* preenche as sublistas temporarias copiando os elementos do vetor original*/
    for (int i = 0; i < n1; i++)
        E[i] = vet[esquerda + i];
    for (int j = 0; j < n2; j++)
        D[j] = vet[meio + 1 + j];

    /* inicializa as variaveis para percorrer as sublistas e o vetor original */
    int i = 0, j = 0, k = esquerda;

    /* compara os elementos das duas sublistas e copia o menor elemento para o vetor */
    while (i < n1 && j < n2)
    {
        if (compara_r(E[i], D[j]) <= 0)
        {
            vet[k] = E[i];
            i++;
        }
        else
        {
            vet[k] = D[j];
            j++;
        }
        k++;
    }

    /* caso sobre elementos não processados nas sublistas, esses elementos são copiados para o vetor */
    while (i < n1)
    {
        vet[k] = E[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        vet[k] = D[j];
        j++;
        k++;
    }
}

/* função que implementa o metodo de ordenação merge sort quebrando o
 *  vetor em subpates até que cada sublista contenha apenas um elemento */
void mergeSort(struct racional vet[], int esquerda, int direita)
{
    if (esquerda < direita)
    {
        /* se a sublista tiver mais de um elemento calcula o índice meio */
        int meio = esquerda + (direita - esquerda) / 2;

        /* chama de forma recursiva a função mergeSort para as duas metades do vetor */
        mergeSort(vet, esquerda, meio);
        mergeSort(vet, meio + 1, direita);

        /* mescla as sublistas já ordenadas no vetor */
        merge(vet, esquerda, meio, direita);
    }
}

/* função que soma todos os elementos do vetor */
struct racional somaTodos(struct racional vet[], long tam)
{
    int i;

    struct racional soma;

    if (!vet)
        return cria_r(0, 1);

    soma = vet[0];

    for (i = 1; i < tam; i++)
        soma_r(soma, vet[i], &soma);

    if (!valido_r(soma))
        return cria_r(0, 1);

    return soma;
}

/* programa principal */
int main()
{
    struct racional vet[100];
    int n, i;
    long num, den;

    srand(0);

    do
    {
        scanf("%d", &n);
    } while (n < 0 || n > 100); // limita de 0 a 100

    for (i = 0; i < n; i++)
    {
        scanf("%ld %ld", &num, &den);
        vet[i] = cria_r(num, den);
    }
    imprimeVet(vet, n);

    n = retiraNaN(vet, n);
    imprimeVet(vet, n);

    mergeSort(vet, 0, n - 1);
    imprimeVet(vet, n);

    printf("SOMA = ");
    imprime_r(somaTodos(vet, n));

    printf("\n");

    return 0;
}
