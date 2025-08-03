#include <stdio.h>

int main()
{

    /*
    printf("char - %lu\n", sizeof(char));
    printf("short - %lu\n", sizeof(short int));
    printf("int - %lu\n", sizeof(int));
    printf("unsignged - %lu\n", sizeof(unsigned));
    printf("long int - %lu\n", sizeof(long int));
    */

   
   /*
   Exercicios 1
   #define max 10

   int V1[max], V2[max];
   int i,N;


    scanf("%d\n", &N);

   for (i = 0; i < N; i++)
   {
        scanf("%d", &V1[i]);
   }

    for (i = 0; i < N; i++)
    {
        printf(" %d\t", V1[i]);
    }
    
    printf("\n\n");

    for (i = 0; i < N; i++)
    {
        V2[i] = V1[i];
    }

    for ( i = 1; i <= N; i++)
    {
        V1[i] = V2[N - i];
    }

    for (i = 1; i <= N; i++)
    {
        printf(" %d\t", V1[i]);
    }
    
    
    


    printf("\n\n");

    return 0;
   */
   

    #define max 10

   int V[max];
   int i,N,cont;
   float M;
    cont = 0;

    scanf("%d\n", &N);

   for (i = 0; i < N; i++)
   {
        scanf("%d", &V[i]);
   }

    for ( i = 0; i < N; i++)
    {
        cont = cont + V[i];
    }

    M = (float)cont/N;

    printf("Média dos valores: %f\n", M);
    

    return 0;
}