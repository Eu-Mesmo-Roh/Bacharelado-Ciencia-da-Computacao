//criar a lista

#include <stdio.h>
#include <stdlib.h>

struct porcao{
    int id;
    int uni;
    struct porcao *prox;
};

struct lista
{
    struct porcao *prim;
    struct porcao *ult;
};


int main()
{

    //criando a lista
    struct lista *circular = malloc(sizeof(struct lista));

    if(circular == NULL)
    {
        printf("erro ao criar a lista");
        return 1;
    }

    circular->prim = NULL;
    circular->ult = NULL;
    

    //cria uma nova porção
    struct porcao *novo = (struct porcao*)malloc(sizeof(struct porcao));

    if(novo == NULL)
    {
        printf("\n\nerro ao inserir porção");
        return 1;
    }

    //insere as informações da porção
    printf("\n\ndigite o id da porção: ");
  
    scanf("%d", &novo->id);
    
    printf("\n\ndigite as unidades da porção: ");
    scanf("%d", &novo->uni);
    
    //corrige os ponteiros
    circular->prim = novo;
    circular->ult = novo;



    struct porcao *novo2 = (struct porcao*)malloc(sizeof(struct porcao));

    if(novo2 == NULL)
    {
        printf("\n\nerro ao inserir porção");
        return 1;
    }

    //insere uma nova porcao
    //insere as informações da porção
    printf("\n\ndigite o id da porção: ");
  
    scanf("%d", &novo2->id);
    
    printf("\n\ndigite as unidades da porção: ");
    scanf("%d", &novo2->uni);
    
    //corrige os ponteiros
    circular->ult->prox = novo2;
    circular->ult = novo2;
    novo2->prox = circular->prim;


    printf("\n\n\nLista de comidas");

    while(circular->prim != circular->ult)
    {
        printf("\n\nid da porcao: %d", circular->prim->id);
        printf("\n\nunidades da porcao: %d\n\n", circular->prim->uni);

        circular->prim = circular->prim->prox;
    }

    printf("\n\nid da porcao: %d", circular->ult->id);
    printf("\n\nunidades da porcao: %d\n\n", circular->ult->uni);

}
