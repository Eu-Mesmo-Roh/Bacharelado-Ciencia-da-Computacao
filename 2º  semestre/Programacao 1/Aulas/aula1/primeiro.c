/* um programa simples */

#include <stdio.h>

int globalVar = 10; // variavel global

void funcao1()
{
    globalVar = 20;
    printf("valor da variavel globalvar em funcao1: %d\n", globalVar);

}

int main()
{
    funcao1();
    printf("valor da variavel globalvar em main: %d\n", globalVar);
    return 0;

}