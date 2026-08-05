#include "ordena.h"

void trocar(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int particiona(int V[], int inicio, int fim) 
{
    int m,i;

    m = V[fim];     // Define o pivô como o último elemento
    i = inicio - 1;

    for (int j = inicio; j < fim; j++) 
    {
        if (V[j] <= m)  // Se o elemento for menor ou igual ao pivô
        {
            i++;
            trocar(&V[i], &V[j]);   // Coloca à esquerda
        }
    }
    trocar(&V[i + 1], &V[fim]);     // Coloca o pivô na posição correta
    return i + 1;
}

void quicksort(int V[], int inicio, int fim)
{
    int m;

    if (inicio < fim) 
    {
        m = particiona(V, inicio, fim);     // Particiona o vetor
        quicksort(V, inicio, m - 1);        // Ordena parte esquerda
        quicksort(V, m + 1, fim);           // Ordena parte direita
    }
}

int ler_run(FILE *entrada, int *armazenamento) 
{
    int cont;

    cont = 0;
    while (cont < TAMANHO_MAXIMO && fscanf(entrada, "%d", &armazenamento[cont]) == 1) 
        cont++;

    return cont;    // Retorna quantos elementos foram lidos
}

void escrever_run(int *armazenamento, int tamanho, int run_num) 
{
    FILE *saida;
    char filename[30];

    sprintf(filename, "runs/run%d.txt", run_num);   // Cria o nome do arquivo
    saida = fopen(filename, "w");                   // Abre o arquivo para escrita
    
    if (!saida) 
    {
        perror("Erro ao abrir arquivo para escrita");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < tamanho; i++) 
        fprintf(saida, "%d\n", armazenamento[i]);   // Escreve um número por linha

    fclose(saida);      // Fecha o arquivo
}

void gerar_runs(const char *arquivo_entrada) 
{
    int *armazenamento, run_num, cont;
    FILE *in;

    // Cria o diretório "runs" se ele não existir
    struct stat st = {0};
    if (stat("runs", &st) == -1) 
    {
        mkdir("runs", 0700);
    }

    in = fopen(arquivo_entrada, "r");   // Abre o arquivo de entrada
    if (!in) 
    {
        perror("Erro ao abrir arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    // Aloca memória para armazenar os blocos
    armazenamento = malloc(TAMANHO_MAXIMO * sizeof(int));
    if (!armazenamento) 
    {
        perror("Erro ao alocar memória");
        fclose(in);
        exit(EXIT_FAILURE);
    }

    run_num = 1;
    while (1) 
    {
        cont = ler_run(in, armazenamento);  // Lê um bloco

        if (cont == 0) break;   // Se não leu nada, fim do arquivo

        quicksort(armazenamento, 0, cont - 1);  // Ordena o bloco
        escrever_run(armazenamento, cont, run_num); // Salva como uma run
        run_num++;
    }

    free(armazenamento);
    fclose(in);
}

int contar_runs() 
{
    int cont;
    struct dirent *entrada;
    DIR *dir;

    dir = opendir("runs");  // Abre o diretório
    if (!dir) return 0;
    
    cont = 0;
    while ((entrada = readdir(dir)) != NULL) 
    {
        // Verifica se o nome do arquivo contém "run" e termina com ".txt"
        if (strstr(entrada->d_name, "run") != NULL && strstr(entrada->d_name, ".txt") != NULL) 
            cont++;
    }

    closedir(dir);  // Fecha o diretório
    return cont;    // Retorna o número de arquivos encontrados
}
