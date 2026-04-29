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



// Função auxiliar recebe o FILE* já aberto. O [[nodiscard]] do C23 garante que o retorno seja verificado.
static int realocar_conteudo(Library *lib, FILE *arquivo, int index, long tamanho_novo, long tamanho_velho)
{
    long delta = tamanho_novo - tamanho_velho;

    if (delta == 0) return 0; // Sem necessidade de realocação

    long inicio_empurrao;
    if (index < lib->count - 1)
        inicio_empurrao = lib->docs[index + 1].offset; 
    else
        inicio_empurrao = lib->docs[index].offset + lib->docs[index].size; 

    fseek(arquivo, 0, SEEK_END);
    long fim_arquivo = ftell(arquivo);
    long bytes_para_empurrar = fim_arquivo - inicio_empurrao;

    char buffer[BUFFER_SIZE];

    if (delta > 0) {
        long bytes_restantes = bytes_para_empurrar;
        long pos_leitura = fim_arquivo;

        while (bytes_restantes > 0) {
            long tamanho_bloco = (bytes_restantes < BUFFER_SIZE) ? bytes_restantes : BUFFER_SIZE;
            
            pos_leitura -= tamanho_bloco;
            fseek(arquivo, pos_leitura, SEEK_SET);
            fread(buffer, 1, tamanho_bloco, arquivo);

            fseek(arquivo, pos_leitura + delta, SEEK_SET);
            fwrite(buffer, 1, tamanho_bloco, arquivo);

            bytes_restantes -= tamanho_bloco;
        }
    } else if (delta < 0) {
        long bytes_lidos_total = 0;
        long pos_leitura = inicio_empurrao;

        while (bytes_lidos_total < bytes_para_empurrar) {
            long tamanho_bloco = ((bytes_para_empurrar - bytes_lidos_total) < BUFFER_SIZE) ? 
                                (bytes_para_empurrar - bytes_lidos_total) : BUFFER_SIZE;
            
            fseek(arquivo, pos_leitura, SEEK_SET);
            fread(buffer, 1, tamanho_bloco, arquivo);

            fseek(arquivo, pos_leitura + delta, SEEK_SET);
            fwrite(buffer, 1, tamanho_bloco, arquivo);

            pos_leitura += tamanho_bloco;
            bytes_lidos_total += tamanho_bloco;
        }
    }

    // Atualiza os offsets dos vizinhos (o do próprio index será mantido)
    for (int i = index + 1; i < lib->count; i++)
        lib->docs[i].offset += delta;
    
    return 0; // Retorna sucesso
}

int gbv_add(Library *lib, const char *archive, const char *docname)
{
    if(strcmp(archive, docname) == 0)
    {
        printf("Erro: O nome do documento não pode ser igual ao nome da biblioteca.\n");
        return 0;
    }

    FILE *doc = fopen(docname, "rb");
    if (!doc) {
        printf("Erro ao abrir documento %s\n", docname);
        return -1;
    }

    // OBTÉM O TAMANHO E VOLTA O CURSOR PRO COMEÇO!
    fseek(doc, 0, SEEK_END);
    long tam_doc = ftell(doc); 
    rewind(doc); 

    FILE *arquivo = fopen(archive, "rb+");
    if (!arquivo) {
        fclose(doc);
        printf("Erro ao abrir biblioteca %s\n", archive);
        return -1; 
    }

    int count = 0;
    long diretorio = 0;
    fread(&count, sizeof(int), 1, arquivo);
    fread(&diretorio, sizeof(long), 1, arquivo);

    int index = -1;
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->docs[i].name, docname) == 0) {
            index = i;
            break;
        }
    }

    long offset_destino;

    if (index != -1) {
        // --- SUBSTITUIÇÃO FÍSICA ---
        offset_destino = lib->docs[index].offset; 
        
        if (tam_doc != lib->docs[index].size) {
            // Passa o arquivo já aberto e verifica a realocação
            if (realocar_conteudo(lib, arquivo, index, tam_doc, lib->docs[index].size) != 0) {
                fclose(doc);
                fclose(arquivo);
                return -1; 
            }
        }

        lib->docs[index].size = tam_doc;
        lib->docs[index].date = time(NULL);
        // O offset original é preservado
    } else {
        // --- INSERÇÃO DE NOVO DOCUMENTO ---
        offset_destino = diretorio; 

        lib->docs = (Document *)realloc(lib->docs, (lib->count + 1) * sizeof(Document));
        strncpy(lib->docs[lib->count].name, docname, MAX_NAME - 1);
        lib->docs[lib->count].name[MAX_NAME - 1] = '\0'; 
        lib->docs[lib->count].size = tam_doc;
        lib->docs[lib->count].date = time(NULL); // C23
        lib->docs[lib->count].offset = offset_destino;
        lib->count++;
    }

    // Grava o conteúdo do arquivo novo no espaço garantido
    fseek(arquivo, offset_destino, SEEK_SET);
    char buffer[BUFFER_SIZE];
    size_t bytes_lidos;
    while ((bytes_lidos = fread(buffer, 1, BUFFER_SIZE, doc)) > 0) {
        fwrite(buffer, 1, bytes_lidos, arquivo);
    }
    fclose(doc);

    // O novo diretório SEMPRE vai começar logo após o fim físico do último documento armazenado
    long novo_diretorio = sizeof(int) + sizeof(long);
    for (int i = 0; i < lib->count; i++) {
        long fim_deste_arquivo = lib->docs[i].offset + lib->docs[i].size;
        if (fim_deste_arquivo > novo_diretorio) {
            novo_diretorio = fim_deste_arquivo;
        }
    }
    
    // Atualiza o superbloco
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&lib->count, sizeof(int), 1, arquivo);
    fwrite(&novo_diretorio, sizeof(long), 1, arquivo);

    // Reescreve o diretório na nova posição
    fseek(arquivo, novo_diretorio, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, arquivo);

    printf("Documento %s adicionado/substituido com sucesso.\n", docname);
    return 0; 
}


int gbv_remove(Library *lib, const char *docname)
{
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

    // Desloca todos os elementos à direita do do removido uma posição para a esquerda
    for(int i = index; i < lib->count; i++)
        lib->docs[i] = lib->docs[i + 1];
    
    lib->count--;

    // Reduz o tamanho do vetor de documentos
    if(lib->count > 0)
        lib->docs = (Document *)realloc(lib->docs, lib->count * sizeof(Document));
    else
    {
        free(lib->docs);
        lib->docs = NULL;
    }

    // Atualiza o arquivo (Superbloco e Diretorio)
    FILE *arquivo = fopen(container_atual, "rb+");

    if(!arquivo)
        return -1;

    int count_antigo;
    long diretorio;

    fread(&count_antigo, sizeof(int), 1, arquivo);
    fread(&diretorio, sizeof(long), 1, arquivo);

    // Atualiza apenas a contagem
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&lib->count, sizeof(int), 1, arquivo);

    // Reescreve o diretório
    if(lib->count > 0)
    {
        fseek(arquivo, diretorio, SEEK_SET);
        fwrite(lib->docs, sizeof(Document), lib->count, arquivo);
    }

    fclose(arquivo);
    printf("Documento %s removido com sucesso.\n", docname);

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
        
        printf("\ec"); 

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

//Funções para ajudar na ordenação
static int comparar_nome(const void *a, const void *b)
{
    Document *docA = (Document *)a;
    Document *docB = (Document *)b;

    // O strcmp retorna <0 se docA < docB, 0 se iguais, >0 se docA > docB
    return strcmp(docA->name, docB->name);
}

static int comparar_tamanho(const void *a, const void *b)
{
    Document *docA = (Document *)a;
    Document *docB = (Document *)b;

    // Retorna negativo se docA < docB, 0 se iguais, positivo se docA > docB
    return (docA->size - docB->size);
}

static int comparar_data(const void *a, const void *b)
{
    Document *docA = (Document *)a;
    Document *docB = (Document *)b;

    // Retorna negativo se A for mais antigo que B
    return (docA->date - docB->date);
}
//Fim das funções de comparação


int gbv_order(Library *lib, const char *archive, const char *criteria)
{
    if(lib->count == 0)
    {
        printf("A biblioteca está vazia. Nada para ordenar.\n");
        return 0; // Sucesso, mas sem documentos
    }

    // Escolhe a função de comparação com base no critério
    if(strcmp(criteria, "nome") == 0)
        qsort(lib->docs, lib->count, sizeof(Document), comparar_nome);
    else if(strcmp(criteria, "tamanho") == 0)
        qsort(lib->docs, lib->count, sizeof(Document), comparar_tamanho);
    else if(strcmp(criteria, "data") == 0)
        qsort(lib->docs, lib->count, sizeof(Document), comparar_data);
    else
    {
        printf("Erro: Critério '%s' invalido. Use: nome, tamanho ou data.\n", criteria);
        return -1; // Critério inválido
    }

    // Após ordenar o vetor em memória, reescreve o diretório no arquivo
    FILE *arquivo = fopen(archive, "rb+");

    if(!arquivo)
        return -1;
    
    int count;
    long diretorio;

    // Lê o super bloco para obter o inicio do diretório
    fread(&count, sizeof(int), 1, arquivo);
    fread(&diretorio, sizeof(long), 1, arquivo);

    // Escreve o diretório ordenado no arquivo
    fseek(arquivo, diretorio, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, arquivo);

    fclose(arquivo);
    printf("Documentos ordenados por %s com sucesso.\n", criteria);

    gbv_list(lib); // Exibe a lista ordenada

    return 0; // Sucesso
}