#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    char nome[50];
    int idade;
    char sexo;
    int cpf;
    float altura;
}pessoa;

int main()
{
    pessoa *p;

    p = malloc(sizeof(pessoa));

    if(p == NULL)
    {
        printf("Erro ao alocar memoria");
        return -1;
    }

    strcpy(p->nome, "Ronaldo Pereira");
    p->idade = 21;
    p->sexo = 'M';
    p->cpf = 123456789;
    p->altura = 1.78;

    printf("Nome: %s\n", p->nome);
    printf("Idade: %d\n", p->idade);
    printf("Sexo: %c\n", p->sexo);
    printf("CPF: %d\n", p->cpf);
    printf("Altura: %.2f\n", p->altura);

    free(p);

    return 0;
}