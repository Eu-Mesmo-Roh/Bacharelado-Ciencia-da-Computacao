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
    long cpf;
}pessoa;

typedef struct
{
    pessoa *elementos[TAM_DICT];
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

[[nodiscard]] bool remover(dicionario* dict, long cpf)
{
    int esquerda = 0;
    int direita = dict->numItens - 1;
    int indice_encontrado = -1;

    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;

        if(dict->elementos[meio]->cpf == cpf)
        {
            indice_encontrado = meio;
            break;
        }

        else if (dict->elementos[meio]->cpf < cpf)
            esquerda = meio + 1; // Busca na metade direita

        else
            direita = meio - 1; // Busca na metade esquerda
    }

    if (indice_encontrado == -1)
        return false; // Não encontrado
    
    free(dict->elementos[indice_encontrado]); // Libera a memória do elemento removido

    for(size_t i = indice_encontrado; i < dict->numItens - 1; i++)
    {
        dict->elementos[i] = dict->elementos[i + 1]; // Desloca os elementos para preencher o "buraco"
    }

    dict->numItens--; // Decrementa o número de itens
    dict->elementos[dict->numItens] = nullptr; // Opcional: Limpa a última posição

    return true;
}



[[nodiscard]] pessoa* buscar(dicionario* dict, long cpf)
{
    int esquerda = 0;
    int direita = dict->numItens - 1;

    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;

        if (dict->elementos[meio]->cpf == cpf)
            return dict->elementos[meio]; // Encontrado

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

    int opcao;

    do
    {
        printf("\n===== MENU DE PESSOAS =====\n");
        printf("1. Adicionar pessoa\n");
        printf("2. Remover pessoa\n");
        printf("3. Buscar pessoa\n");
        printf("4. Listar pessoas\n");
        printf("0. Sair\n");
        printf("==================================");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
            {
                auto nova_pessoa = (pessoa*)malloc(sizeof(pessoa));

                if (nova_pessoa == nullptr)
                {
                    printf("Erro de memoria!\n");
                    break;
                }

                printf("\n=== Digite os dados da pessoa ===\n");
                printf("Nome: ");

                scanf(" %49[^\n]", nova_pessoa->nome); // Lê uma

                printf("Idade: ");
                scanf("%d", &nova_pessoa->idade);

                printf("Sexo (M/F): ");
                scanf(" %c", &nova_pessoa->sexo);

                printf("CPF: ");
                scanf("%ld", &nova_pessoa->cpf);

                if (adicionar(&meuDict, nova_pessoa))
                    printf("Pessoa adicionada com sucesso!\n");
                else
                {
                    printf("Dicionário cheio! Não foi possível adicionar a pessoa.\n");
                    free(nova_pessoa); // Libera a memória alocada
                }
                break;

            }
            case 2:
            {
                long cpf_remover;
                printf("\nDigite o CPF da pessoa a remover: ");
                scanf("%ld", &cpf_remover);

                if (remover(&meuDict, cpf_remover))
                    printf("Pessoa removida com sucesso!\n");

                else
                    printf("Pessoa com CPF %ld não encontrada.\n", cpf_remover);

                break;
            }
            case 3:
            {
                long cpf_buscar;

                printf("\nDigite o CPF da pessoa a buscar: ");
                scanf("%ld", &cpf_buscar);

                auto resultado = buscar(&meuDict, cpf_buscar);
                if (resultado != nullptr)
                {
                    printf("\nPessoa encontrada:\n");
                    printf("Nome: %s\n", resultado->nome);
                    printf("Idade: %d\n", resultado->idade);
                    printf("Sexo: %c\n", resultado->sexo);
                    printf("CPF: %ld\n", resultado->cpf);
                }
                else
                    printf("Pessoa com CPF %ld não encontrada.\n", cpf_buscar);
                
                break;
            }
            case 4:
            {
                printf("\n=== Lista de Pessoas Cadastradas ===\n");

                if (meuDict.numItens == 0)
                {
                    printf("Nenhuma pessoa cadastrada.\n");
                }
                else
                {
                    for (size_t i = 0; i < meuDict.numItens; i++)
                    {
                        printf("\nPessoa %zu:\n", i + 1);
                        printf("Nome: %s\n", meuDict.elementos[i]->nome);
                        printf("Idade: %d\n", meuDict.elementos[i]->idade);
                        printf("Sexo: %c\n", meuDict.elementos[i]->sexo);
                        printf("CPF: %ld\n", meuDict.elementos[i]->cpf);
                    }
                }
                break;
            }
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Por favor, escolha uma opção válida.\n");
        }
    }
    while (opcao != 0);

    // Liberar memória alocada
    for (size_t i = 0; i < meuDict.numItens; i++)
    {
        free(meuDict.elementos[i]);
    }

    return 0;
}




/* int main()
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
} */
