#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pessoa
{
    char nome[50];
    int idade;
    long cpf;
    struct pessoa *esquerda;
    struct pessoa *direita;
}pessoa;

void preorder(pessoa *raiz)
{
    if(raiz != NULL)
    {
        printf("Nome: %s", raiz->nome);
        printf("Idade: %d\n", raiz->idade);
        printf("CPF: %ld\n\n", raiz->cpf);
        preorder(raiz->esquerda);
        preorder(raiz->direita);
    }
}

void inorder(pessoa *raiz)
{
    if(raiz != NULL)
    {
        inorder(raiz->esquerda);
        printf("Nome: %s", raiz->nome);
        printf("Idade: %d\n", raiz->idade);
        printf("CPF: %ld\n\n", raiz->cpf);
        inorder(raiz->direita);
    }
}

void postorder(pessoa *raiz)
{
    if(raiz != NULL)
    {
        postorder(raiz->esquerda);
        postorder(raiz->direita);
        printf("Nome: %s", raiz->nome);
        printf("Idade: %d\n", raiz->idade);
        printf("CPF: %ld\n\n", raiz->cpf);
    }
}

int main()
{
    pessoa *raiz = NULL;
    pessoa *novo = (pessoa*) malloc(sizeof(pessoa));

    if(novo == NULL)
    {
        printf("Erro ao alocar memoria");
        return 1;
    }

    printf("digite o nome da pessoa: \n");
    fgets(novo->nome, 50, stdin);
    printf("digite a idade da pessoa: \n");
    scanf("%d", &novo->idade);
    printf("digite o cpf da pessoa: \n");
    scanf("%ld", &novo->cpf);
    novo->esquerda = NULL;
    novo->direita = NULL;
    raiz = novo;

    getchar();

    printf("\n\nadicione a segunda pessoa na arvore: \n");
    pessoa *segundo = (pessoa*) malloc(sizeof(pessoa));

    if(segundo == NULL)
    {
        printf("Erro ao alocar memoria");
        return 1;
    }


    printf("\ndigite o nome da pessoa: ");
    fgets(segundo->nome, 50, stdin);
    printf("\ndigite a idade da pessoa: ");
    scanf("%d", &segundo->idade);
    printf("\ndigite o cpf da pessoa: ");
    scanf("%ld", &segundo->cpf);
    segundo->esquerda = NULL;
    segundo->direita = NULL;
    raiz->esquerda = segundo;


    printf("\n\nPessoas encontradas na arvore: \n\n");
    
    preorder(raiz);
    printf("\n\n");

    inorder(raiz);
    printf("\n\n");

    postorder(raiz);
    printf("\n\n");

    return 0;
}