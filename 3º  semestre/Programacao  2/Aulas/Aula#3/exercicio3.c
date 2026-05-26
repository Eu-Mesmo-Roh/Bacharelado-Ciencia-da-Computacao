#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a struct
typedef struct 
{
    char nome[50];
    int idade;
    char cidade[50];
} Pessoa;

int main() 
{
    // Array para armazenar até 100 pessoas
    Pessoa arrayDePessoas[100]; 
    char linha[256];
    int quantidade;

    printf("Digite as linha no formato CSV (ex: Nome,Idade,Cidade):\n");
    printf("Digite a palavra 'sair' para encerrar:\n");
    
    quantidade = 0;

    while (1)
    {
        // Lê a linha do usuario
        if (fgets(linha, sizeof(linha), stdin) != NULL)
        {
            // Removendo o caractere de nova linha que fgets adiciona no final da string
            linha[strcspn(linha, "\n")] = '\0';

            // Verifica se o usuário digitou "sair"
            if (strcmp(linha, "sair") == 0) 
                break; // Encerra o loop
            
            // A primeira chamada do strtok pega a linha original e o delimitador
            char *token = strtok(linha, ",");

            // O laço continua enquanto a função encontrar dados
            if (token != NULL)
            {
                strcpy(arrayDePessoas[quantidade].nome, token);

                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    // Converte a string para inteiro usando atoi
                    arrayDePessoas[quantidade].idade = atoi(token);

                    token = strtok(NULL, ",");
                    if (token != NULL)
                    {
                        strcpy(arrayDePessoas[quantidade].cidade, token);
                        quantidade++;
                    }
                }
            }
        }
    }

    //Ordenar o array de pessoas por nome
    for (int i = 0; i < quantidade - 1; i++) 
    {
        for (int j = 0; j < quantidade - i - 1; j++) 
        {
            if (strcmp(arrayDePessoas[j].nome, arrayDePessoas[j + 1].nome) > 0) 
            {
                Pessoa temp = arrayDePessoas[j];
                arrayDePessoas[j] = arrayDePessoas[j + 1];
                arrayDePessoas[j + 1] = temp;
            }
        }
    }

    // Imprime o array de pessoas ordenado
    printf("\nPessoas ordenadas por nome:\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("Nome %d: %s, Idade: %d, Cidade: %s\n", i + 1, arrayDePessoas[i].nome, arrayDePessoas[i].idade, arrayDePessoas[i].cidade);
    }

    return 0;
}