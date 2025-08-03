/*começando a entender o Scanf*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
/*  
    int x;

    x = 0;
    while (x<10)
    {
        printf("%d\n", x);
        x++;  
    }
*/
/*
    int x,y;
    while (scanf("%d %d", &x, &y)==2 && x<y)
    {
        while (x<=y)
        {
            printf("%d \t", x);
            x++;
        }
        printf("\n");
    }
*/


int num =0;
srand(356);

/*Exemplo de While com atribuição na condição de parada para se gerar 5*/
while ((num = rand() % 10) != 5)
    printf("Número gerado: %d\n", num);

printf("\nSaiu do loop porque o número gerado foi 5.\n");

    return 0;
}