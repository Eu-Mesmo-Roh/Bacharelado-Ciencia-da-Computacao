#include <stdio.h>

/*

double exponencial (int x, int y)
{
    double cont;

    cont = 1;

    for ( int i = 1; i <= y; i++)
    {
        cont = cont * x;
    }

    return cont;

}



int main()
{
    double a;
    int b;

    scanf("%lf %d", &a, &b);

    double resultado = exponencial(a, b);

    printf("%lf", resultado);

    return 0;
}

*/

void troca (int *x, int *y)
{
    int aux;

    aux = *x;
    *x = *y;
    *y = *x;
}

int main()
{
    int a,b;

    scanf("%d %d", a,b);

    printf("valores antes da troca a = %d, b = %d", a,b);

    troca(&a, &b);

    printf("valores depois da troca a = %d, b = %d", a,b);


    return (0);
}