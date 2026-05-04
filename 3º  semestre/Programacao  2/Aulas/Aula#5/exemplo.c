#include <stdio.h>

int main()
{
    int n;
    int buffer[10];


    FILE *file = fopen("input.txt", "rb");

    

    fread(buffer, sizeof(int), 10, file);

    for(int i = 0; i < 10; i++)
    {
        printf("testando o bufer: %d\n\n", buffer[i]);
    }

    fclose(file);
}