#include "ordena.h"

void heapify(int *valores, int *indices, int tamanho, int i)
{
    int menor, esquerda, direita, temp_valor, temp_indice;

    menor = i;
    esquerda = 2 * i + 1;   // Índice do filho à esquerda
    direita = 2 * i + 2;    // Índice do filho à direita

    // Encontra o menor entre o pai e os filhos
    if (esquerda < tamanho && valores[esquerda] < valores[menor]) 
        menor = esquerda;

    if (direita < tamanho && valores[direita] < valores[menor]) 
        menor = direita;

    // Se necessário, troca o pai com o menor filho e continua heapificando
    if (menor != i) 
    {
        // Troca valores
        temp_valor = valores[i];
        valores[i] = valores[menor];
        valores[menor] = temp_valor;
        
        // Troca índices
        temp_indice = indices[i];
        indices[i] = indices[menor];
        indices[menor] = temp_indice;
        
        heapify(valores, indices, tamanho, menor);
    }
}

void construir_heap(int *valores, int *indices, int tamanho) 
{
    // Começa do meio para trás, ajustando os nós internos
    for (int i = tamanho / 2 - 1; i >= 0; i--) 
        heapify(valores, indices, tamanho, i);
}

void intercala_grupo_de_runs(int inicio, int fim, const char *saida_nome)
{
    int k, heap_tam, origem, *valores, *indices;
    FILE **arquivos, *saida;

    k = fim - inicio + 1;
    valores = malloc(k * sizeof(int));
    indices = malloc(k * sizeof(int));

    arquivos = malloc(k * sizeof(FILE *));
    saida = fopen(saida_nome, "w");

    if (!valores || !indices || !arquivos || !saida) 
    {
        perror("Erro de alocação ou abertura de arquivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < k; i++)
    {
        char nome[64];
        sprintf(nome, "runs/run%d.txt", inicio + i);
        arquivos[i] = fopen(nome, "r");

        if (!arquivos[i]) 
        {
            perror("Erro ao abrir run");
            exit(EXIT_FAILURE);
        }

        if (fscanf(arquivos[i], "%d", &valores[i]) != 1) 
            valores[i] = INT_MAX; // Marca fim da run

        indices[i] = i;
    }

    construir_heap(valores, indices, k);

    heap_tam = k;
    while (heap_tam > 0 && valores[0] != INT_MAX) 
    {
        fprintf(saida, "%d\n", valores[0]);
        origem = indices[0];

        if (fscanf(arquivos[origem], "%d", &valores[0]) != 1) 
        {
            valores[0] = valores[heap_tam - 1];
            indices[0] = indices[heap_tam - 1];
            heap_tam--;
        }

        heapify(valores, indices, heap_tam, 0);
    }

    for (int i = 0; i < k; i++) fclose(arquivos[i]);
    fclose(saida);
    free(valores); free(indices); free(arquivos);
}

void intercala_runs_em_fases(int total_runs, const char *saida_final) 
{
    int grupo, fim, k, *valores, *indices, origem, heap_tam;
    char nome_intermediarios[100][64];  // Armazena nomes dos arquivos intermediários
    FILE **arquivos, *saida;
    
    grupo = 0;

    // Cria a pasta intermediarios se não existir
    struct stat st = {0};
    if (stat("intermediarios", &st) == -1)
        mkdir("intermediarios", 0700);

    // Etapa 1: gerar arquivos intermediários

    for (int i = 1; i <= total_runs; i += MAX_RUNS_POR_FASE) 
    {
        // Define o último índice do grupo atual
        fim = i + MAX_RUNS_POR_FASE - 1;
        if (fim > total_runs) fim = total_runs;

        // Gera o nome do arquivo intermediário do grupo atual
        char nome[64];
        sprintf(nome, "intermediarios/intermediario%d.txt", grupo);
        strcpy(nome_intermediarios[grupo], nome);  // Salva para usar na Etapa 2

        intercala_grupo_de_runs(i, fim, nome);
        grupo++;
    }

    // Etapa 2: intercala os arquivos intermediários

    k = grupo;  // Número de arquivos intermediários

    // Aloca vetores para manter os valores atuais da heap e a origem de cada valor
    valores = malloc(k * sizeof(int));
    indices = malloc(k * sizeof(int));
    arquivos = malloc(k * sizeof(FILE *));
    saida = fopen(saida_final, "w");

    if (!valores || !indices || !arquivos || !saida) 
    {
        perror("Erro na fase final");
        exit(EXIT_FAILURE);
    }

    // Abre os arquivos intermediários e lê o primeiro valor de cada um
    for (int i = 0; i < k; i++) 
    {
        arquivos[i] = fopen(nome_intermediarios[i], "r");
        if (!arquivos[i]) 
        {
            perror("Erro ao abrir intermediário");
            exit(EXIT_FAILURE);
        }

        // Lê o primeiro valor; se não conseguir, coloca INT_MAX para ignorar
        if (fscanf(arquivos[i], "%d", &valores[i]) != 1) 
            valores[i] = INT_MAX;

        indices[i] = i;
    }

    // Constrói a min-heap com os primeiros valores de cada arquivo intermediário
    construir_heap(valores, indices, k);
    heap_tam = k;

    // Enquanto ainda houver elementos válidos na heap
    while (heap_tam > 0 && valores[0] != INT_MAX)
    {
        fprintf(saida, "%d\n", valores[0]);     // Escreve o menor valor da heap no arquivo de saída
        origem = indices[0];       // Identifica de qual arquivo esse menor valor veio

        // Tenta ler o próximo número do mesmo arquivo
        if (fscanf(arquivos[origem], "%d", &valores[0]) != 1)
        {
            // Se não conseguir, remove o elemento da heap
            valores[0] = valores[heap_tam - 1];
            indices[0] = indices[heap_tam - 1];
            heap_tam--;
        }

        // Reorganiza a heap após a substituição
        heapify(valores, indices, heap_tam, 0);
    }

    // Fecha todos os arquivos abertos
    for (int i = 0; i < k; i++) fclose(arquivos[i]);
    fclose(saida);

    free(valores); free(indices); free(arquivos);   // Libera a memória alocada
}
