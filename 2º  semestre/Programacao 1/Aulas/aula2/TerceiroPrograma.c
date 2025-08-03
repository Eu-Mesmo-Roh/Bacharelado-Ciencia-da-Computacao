#include <stdio.h>
#define TAM_STRING 10

int main()
{
    int i = 10;
    float f = 2.5;
    char string[TAM_STRING] = "hi";
    char letra = 'a';
    printf ("o inteiro é %d\n", i);
    printf ("o float é %f\n", f);
    printf ("A string é = %s e o caracter é = %c\n", string, letra);

    letra = 110; /*o tipo caracter na verdade é um inteiro*/
    printf ("%c %d\n", letra, letra);

    return 0;

}