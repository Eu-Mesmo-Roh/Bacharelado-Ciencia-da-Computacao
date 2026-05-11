#ifndef GBV_H
#define GBV_H

#include <time.h>

#define MAX_NAME 256
#define BUFFER_SIZE 512   // tamanho fixo do buffer em bytes

// Estrutura de metadados de cada documento
typedef struct {
    char name[MAX_NAME];   // nome do documento
    long size;             // tamanho em bytes
    time_t date;           // data de inserção
    long offset;           // posição no container
} Document;

// Estrutura que representa a biblioteca (diretório em memória)
typedef struct {
    Document *docs;        // vetor dinâmico de documentos
    int count;             // número de documentos
} Library;

// Funções que voce deve implementar em gbv.c

// Função para criar um novo arquivo de biblioteca
int gbv_create(const char *filename);

// Função para abrir a biblioteca e carregar o diretório para a memoria
int gbv_open(Library *lib, const char *filename); 

// Função para adicionar um documento à biblioteca
int gbv_add(Library *lib, const char *archive, const char *docname);

// Função para remover um documento da biblioteca
int gbv_remove(Library *lib, const char *docname);

// Função para listar os documentos na biblioteca
int gbv_list(const Library *lib);

// Função para visualizar o conteúdo de um documento específico
int gbv_view(const Library *lib, const char *docname);

// Função para ordenar os documentos da biblioteca por um critério específico
int gbv_order(Library *lib, const char *archive, const char *criteria);

#endif

