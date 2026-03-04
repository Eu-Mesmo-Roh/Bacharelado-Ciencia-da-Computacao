#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*fazendo alterações para C2023*/


/* #define TAM_DICIONARIO 100
#define MAX_CHAR 50 */

/*as funções a baixo substituem os defines acima no C2023*/
constexpr int MAX_CHAR = 50;
constexpr int TAM_DICT = 100;

typedef struct
{
    char nome[MAX_CHAR];
    int idade;
    char sexo;
    int cpf;
}pessoa;

typedef struct
{
    pessoa* elementos[TAM_DICT];
    size_t numItens;
}dicionario;

[[nodiscard]] bool adicionar(dicionario* dict, pessoa* p)
{
    if (dict->numItens >= TAM_DICT)
        return false; // Dicionário cheio
    
    size_t i;

    i = dict->numItens;

    // Desloca os elementos maiores que o novo CPF para a direita 
    // para "abrir um buraco" na posição correta

    while (i > 0 && dict->elementos[i - 1]->cpf > p->cpf)
    {
        dict->elementos[i] = dict->elementos[i - 1];
        i--;
    }

    // Insere o novo elemento na posição correta
    dict->elementos[i] = p;
    dict->numItens++;

    return true;
}

[[nodiscard]] pessoa* buscar(dicionario* dict, int cpf)
{
    size_t esquerda = 0;
    size_t direita = dict->numItens;

    while (esquerda <= direita)
    {
        size_t meio = esquerda + (direita - esquerda) / 2;

        if (dict->elementos[meio]->cpf == cpf)
            return dict->elementos[meio]; // Encconditionontrado

        else if (dict->elementos[meio]->cpf < cpf)
            esquerda = meio + 1; // Busca na metade direita

        else
            direita = meio - 1; // Busca na metade esquerda
    }

    return nullptr; // Não encontrado
}

int main()
{
    dicionario meuDict = {}; // Inicializa o dicionário vazio

    auto p1 = (pessoa*)malloc(sizeof(pessoa));
    strcpy(p1->nome, "Ronaldo");
    p1->idade = 21;
    p1->sexo = 'M';
    p1->cpf = 123456789;

    auto p2 = (pessoa*)malloc(sizeof(pessoa));
    strcpy(p2->nome, "Maria");
    p2->idade = 30;
    p2->sexo = 'F';
    p2->cpf = 987654321;

    auto p3 = (pessoa*)malloc(sizeof(pessoa));
    strcpy(p3->nome, "João");
    p3->idade = 25;
    p3->sexo = 'M';
    p3->cpf = 555555555;

    if (adicionar(&meuDict, p1)) 
        printf("Pessoa 1 adicionada!\n");
    
    if (adicionar(&meuDict, p2))
        printf("Pessoa 2 adicionada!\n");
    
    if (adicionar(&meuDict, p3))
        printf("Pessoa 3 adicionada!\n");
    
    printf("Total de pessoas cadastradas: %zu\n\n", meuDict.numItens);

    //testando busca

    int cpfBusca = 987654321;
    auto resultado = buscar(&meuDict, cpfBusca);

    if(resultado != nullptr)
    {
        printf("Pessoa encontrada:\n");
        printf("Nome: %s\n", resultado->nome);
        printf("Idade: %d\n", resultado->idade);
        printf("Sexo: %c\n", resultado->sexo);
        printf("CPF: %d\n", resultado->cpf);
    }
    else
    {
        printf("Pessoa com CPF %d não encontrada.\n", cpfBusca);
    }

    // Liberar memória alocada
    free(p1);
    free(p2);
    free(p3);

    return 0;
}
