#include <stdio.h>

int main()
{
    int x = 10;
    int y = 15;
    {
        int y = 20;
        int z = 5;

        printf("valor de x: %d\n", x); /*x é acessivel aqui*/ 

        printf("valor de y: %d\n", y); /*y é acessivel aqui*/ 

        printf("agora um novo valor z: %d\n", z); /*z é acessivel aqui*/ 
    }

    printf("valor de y fora do código: %d\n", y);
    
    return 0;

}