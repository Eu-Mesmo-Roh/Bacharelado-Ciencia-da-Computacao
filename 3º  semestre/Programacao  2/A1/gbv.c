#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gbv.h"
#include "util.h"

// Variável estática para guardar o nome do container aberto, para ser usada em outras funções
static char container_atual[MAX_NAME] = "";

int gbv_create(const char *filename)
{
    FILE *arquivo = fopen(filename, "wb");
    if(!arquivo)
        return -1; // Erro ao criar o arquivo
    
    int count = 0;

    long diretorio = sizeof(int) + sizeof(long); // espaço para count e diretorio

    // Escreve o super bloco
    fwrite(&count, sizeof(int), 1, arquivo);
    fwrite(&diretorio, sizeof(long), 1, arquivo);

    fclose(arquivo);
    return 0; // Sucesso
}

int gbv_open(Library *lib, const char *filename)
{
    strncpy(container_atual, filename, MAX_NAME - 1);


    FILE *arquivo = fopen(filename, "rb+");

    if(!arquivo)
    {
        // Se o arquivo não existe, cria um novo
        if(gbv_create(filename) != 0)
            return -1; // Erro ao criar a biblioteca
        arquivo = fopen(filename, "rb+");
        if(!arquivo)
            return -1; // Erro ao abrir a biblioteca
    }

    int count = 0;
    long diretorio = 0;

    // Lê o super bloco
    fread(&count, sizeof(int), 1, arquivo);
    fread(&diretorio, sizeof(long), 1, arquivo);

    lib->count = count;

    // Aloca memória para os documentos
    if(count > 0)
    {
        //Aloca memória para os documentos
        lib->docs = (Document *)malloc(count * sizeof(Document));
        if(!lib->docs)
        {
            fclose(arquivo);
            return -1; // Erro de memória
        }

        // Pula para o byte onde o diretório começa
        fseek(arquivo, diretorio, SEEK_SET);

        // Lê todos os metadados de uma vez para o vetor alocado
        fread(lib->docs, sizeof(Document), count, arquivo);
    }
    else
        lib->docs = NULL; // Nenhum documento na biblioteca
    
    fclose(arquivo);
    return 0; // Sucesso
}

int gbv_add(Library *lib, const char *archive, const char *docname)
{
    // Abre o documento de origem para leitura
    FILE *doc = fopen(docname, "rb");
    if(!doc)
    {
        printf("Erro ao abrir documento %s\n", docname);
        return -1; // Erro ao abrir o documento
    }

    // Abre a biblioteca para leitura e escrita
    FILE *arquivo = fopen(archive, "rb+");
    if(!arquivo)
    {
        fclose(doc);
        printf("Erro ao abrir biblioteca %s\n", archive);
        return -1; // Erro ao abrir a biblioteca
    }

    // Lê o super bloco para saber onde o diretório começa
    int count = 0;
    long diretorio = 0;
    fread(&count, sizeof(int), 1, arquivo);
    fread(&diretorio, sizeof(long), 1, arquivo);

    // Verifica se o documento já existe na biblioteca
    int index = -1;
    for(int i = 0; i < lib->count; i++)
    {
        if (strcmp(lib->docs[i].name, docname) == 0)
        {
            index = i;
            break;
        }
    }

    // Move o ponteiro para o offset do diretorio antigo (para sobrescreve-lo)
    fseek(arquivo, diretorio, SEEK_SET);
    long novo_documento = diretorio; // O novo diretório começa aqui

    // Copia os dados respeitando o limite rígido de buffer
    char buffer[BUFFER_SIZE];
    size_t bytes_lidos;
    long tam_doc = 0;

    while ((bytes_lidos = fread(buffer, 1, BUFFER_SIZE, doc)) > 0)
    {
        fwrite(buffer, 1, bytes_lidos, arquivo);
        tam_doc += bytes_lidos;
    }
    fclose(doc);

    // O novo diretório vai começar após os dados do documento
    long novo_diretorio = novo_documento + tam_doc;

    // Atualiza o super bloco com o novo diretório
    if(index != -1)
    {
        // Documento já existe, atualiza o metadado
        lib->docs[index].size = tam_doc;
        lib->docs[index].date = time(NULL);
        lib->docs[index].offset = novo_documento;    
    }
    else
    {
        // Novo documento, adiciona ao vetor e atualiza o count
        lib->docs = (Document *)realloc(lib->docs, (lib->count +1) * sizeof(Document));
        strncpy(lib->docs[lib->count].name, docname, MAX_NAME - 1);
        lib->docs[lib->count].name[MAX_NAME - 1] = '\0'; // Garantir terminação nula
        lib->docs[lib->count].size = tam_doc;
        lib->docs[lib->count].date = time(NULL);
        lib->docs[lib->count].offset = novo_documento;
        lib->count++;
    }

    // Escreve o diretorio atualizado no novo final do arquivo
    fseek(arquivo, novo_diretorio, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, arquivo);

    // Atualiza o super bloco com o novo count e novo diretorio
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&lib->count, sizeof(int), 1, arquivo);
    fwrite(&novo_diretorio, sizeof(long), 1, arquivo);

    fclose(arquivo);
    printf("Documento %s adicionado com sucesso.\n", docname);
    return 0; // Sucesso
}

int gbv_list(const Library *lib)
{
    if(lib->count == 0)
    {
        printf("A biblioteca está vazia.\n");
        return 0; // Sucesso, mas sem documentos
    }

    printf("%-30s %-10s %-20s %-10s\n", "Nome", "Tamanho", "Data de Insercao", "Offset");
    printf("--------------------------------------------------------------------------------\n");

    for(int i = 0; i < lib->count; i++)
    {
        char date_str[64];
        // Usa a função utilitária para formatar a data
        format_date(lib->docs[i].date, date_str, sizeof(date_str));
        printf("%-30s %-10ld %-20s %-10ld\n", lib->docs[i].name, lib->docs[i].size, date_str, lib->docs[i].offset);
    }
    return 0; // Sucesso
}


//int gbv_remove(Library *lib, const char *docname)
//int gbv_list(const Library *lib)

int gbv_view(const Library *lib, const char *docname)
{
    // Procura o documento no diretório
    int index = -1;
    for(int i = 0; i < lib->count; i++)
    {
        if(strcmp(lib->docs[i].name, docname) == 0)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        printf("Documento %s não encontrado na biblioteca.\n", docname);
        return -1; // Documento não encontrado
    }

    // Abre o Container
    FILE *arquivo = fopen (container_atual, "rb");
    if(!arquivo)
    {
        printf("Erro ao abrir a biblioteca %s\n", container_atual);
        return -1; // Erro ao abrir a biblioteca
    }

    long offset_inicial = lib->docs[index].offset;
    long tamanho = lib->docs[index].size;

    // Calculando o total de blocos necessários
    long total_blocos = (tamanho + BUFFER_SIZE - 1) / BUFFER_SIZE;
    if(total_blocos == 0)
        total_blocos = 1; // Garantir que pelo menos um bloco seja lido para arquivos vazios
    
    long bloco_atual = 0;
    char buffer[BUFFER_SIZE];
    char comando[10] = "n"; // Comando para avançar para o próximo bloco

    while(comando[0] != 'q')
    {
        // Trava os limites da navegação
        if(bloco_atual < 0)
            bloco_atual = 0;

        if(bloco_atual >= total_blocos)
            bloco_atual = total_blocos - 1;

        // Calcula o offset para o bloco atual
        long offset_leitura = offset_inicial + (bloco_atual * BUFFER_SIZE);
        fseek(arquivo, offset_leitura, SEEK_SET);

        // Define quantos bytes ler, considerando o último bloco que pode ser menor que BUFFER_SIZE
        long bytes_para_ler = BUFFER_SIZE;

        // Se for o último bloco, ajusta para ler apenas o que resta do documento
        if(bloco_atual == total_blocos - 1)
        {
            bytes_para_ler = tamanho % BUFFER_SIZE;
            if(bytes_para_ler == 0 && tamanho > 0)
                bytes_para_ler = BUFFER_SIZE;
        }

        // Lê e imprime o bloco
        size_t bytes_lidos = fread(buffer, 1, bytes_para_ler, arquivo);

        printf("\n=== %s (Bloco %ld de %ld) ===\n", docname, bloco_atual + 1, total_blocos);
        for(size_t i = 0; i < bytes_lidos; i++) {
            putchar(buffer[i]);
        }
        printf("\n===================================\n");

        // Solicita o comando do usuário
        printf("Navegação: (n) Próximo bloco, (p) Bloco anterior, (q) Sair: ");
        if(fgets(comando, sizeof(comando), stdin) == NULL)
            break;
        
        if(comando[0] == 'n')
        {
            if(bloco_atual < total_blocos - 1)
                bloco_atual++;
            else
                printf("\n\n\nVocê já está no último bloco.\n");
        }
        else if(comando[0] == 'p')
        {
            if(bloco_atual > 0)
                bloco_atual--;
            else
                printf("\n\n\nVocê já está no primeiro bloco.\n");
        }
    }

    fclose(arquivo);
    return 0; // Sucesso
}

//int gbv_order(Library *lib, const char *archive, const char *criteria)