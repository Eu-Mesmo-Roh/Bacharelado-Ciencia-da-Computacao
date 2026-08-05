#ifndef ORDENA_H
#define ORDENA_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

#define TAMANHO_MAXIMO 1000  // Tamanho máximo de uma run (bloco a ser lido e ordenado)
#define MAX_RUNS_POR_FASE 1000


// =======================
// Funções auxiliares
// =======================

// Troca o conteúdo de dois inteiros
void trocar(int *a, int *b);  

// Particiona vetor para o quicksort
int particiona(int V[], int inicio, int fim);  



// =======================
// Funções de Ordenação
// =======================

// Algoritmo de ordenação QuickSort
void quicksort(int V[], int inicio, int fim);

// Lê uma run do arquivo de entrada
int ler_run(FILE *input, int *buffer);

// Escreve uma run ordenada em um arquivo runs/runN.txt
void escrever_run(int *buffer, int size, int run_index);

// Gera runs a partir do arquivo de entrada
void gerar_runs(const char *input_file);



// =======================
// Funções de Intercalação
// =======================

// Função auxiliar para manter a propriedade de heap mínimo
void heapify(int *valores, int *indices, int tamanho, int i);  

// Constroi o heap mínimo inicial
void construir_heap(int *valores, int *indices, int tamanho);  


// =======================
// Função principal de intercalação dos arquivos de runs
// =======================

// Função que intercala e agrupa runs 
void intercala_grupo_de_runs(int inicio, int fim, const char *saida_nome);

// função que intercala os grupos
void intercala_runs_em_fases(int total_runs, const char *saida_final);

// Conta quantos arquivos de run existem
int contar_runs();


#endif