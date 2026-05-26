/* 
dividindo o problema em blocos

1. Abrir arquivos com segurança
2. Ler o arquivo palavra por palavra
3. Armazenar e contar as palavras
4. Descobrir a palavra com mais ocorrencia e limpar tudo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodofrequencia
{
    char palavras[101];
    int contagem;
    struct nodofrequencia *prox;
};

int main()
{
    //inicializando variaveis e vetores
    char nome_arquivo[256], palavras_lidas[101];
    char palavras_frequentes[101] = "";
    int encontrou, max_frequencia;

    // criando o ponteiro do arquivo
    FILE* arquivo;

    struct nodofrequencia *inicio_lista = NULL;
    struct nodofrequencia *atual = NULL;

    max_frequencia = 0;

    //solicitando o nome do arquivo e atribuindo o ponteiro
    printf("Digite o nome do arquivo texto .txt: ");
    scanf("%255s", nome_arquivo);

    //abrindo o arquivo para a leitura
    arquivo = fopen(nome_arquivo, "r");

    //testando se o ponteiro do aquivo é valido
    if(arquivo == NULL)
    {
        printf("Erro: O Arquivo %s não pode ser aberto.\n", nome_arquivo);
        return 1;
    }

    printf("Arquivo %s aberto com sucesso.\n", nome_arquivo);
    printf("Lendo as palavras do arquivo\n");

    //laço para ler as palavras
    while(fscanf(arquivo, "%100s", palavras_lidas) == 1)
    {
        atual = inicio_lista;
        encontrou = 0;

        //procura pela palavra na lista para fazer a contagem
        while(atual != NULL)
        {
            if(strcmp(atual->palavras, palavras_lidas)== 0)
            {
                atual->contagem++;
                encontrou = 1;
                break;;
            }
            atual = atual->prox;
        }
        
        //se não encontrar temos uma nova palavra e a inserimos na lista
        if(encontrou == 0)
        {
            struct nodofrequencia *novo = (struct nodofrequencia *)malloc(sizeof(struct nodofrequencia));

            strcpy(novo->palavras, palavras_lidas);
            novo->contagem = 1;

            //adiciona o novo nodo no inicio da lista
            novo->prox = inicio_lista;
            inicio_lista = novo;
        }      
    }

    fclose(arquivo);

    max_frequencia = 0;
    
    atual = inicio_lista;

    //procurando pela palavra que aparece com mais frequencia
    while(atual != NULL)
    {
        if(atual->contagem > max_frequencia)
        {
            max_frequencia = atual->contagem;
            strcpy(palavras_frequentes, atual->palavras);
        }

        atual = atual->prox;
    }

    //resultado final da busca
    if(max_frequencia > 0)
    {
        printf("\n\nResultado final\n\n");
        printf("palavra mais frequente é: '%s'\n", palavras_frequentes);
        printf("numero de ocorrencias: %d\n", max_frequencia);
    }
    else
        printf("nenhuma palavra foi encontrada no arquivo");

    atual = inicio_lista;

    //liberando a memoria da lista
    while(atual != NULL)
    {
        struct nodofrequencia *temp = atual;
        atual = atual->prox;
        free(temp);
    }

    return 0;

}