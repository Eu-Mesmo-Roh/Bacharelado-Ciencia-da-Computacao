// programa principal do projeto "The Boys - 2024/2"
// Autor: Ronaldo Pereira da Costa, GRR: 20240514

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "entidades.h"
#include "eventos.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main()
{
  // Inicializa o mundo
  struct mundo_t *mundo = cria_mundo();
  if (!mundo)
  {
    fprintf(stderr, "Erro ao inicializar o mundo.\n");
    return 1;
  }

  // Inicializa a fila de prioridade
  struct fprio_t *fprio = fprio_cria();
  if (!fprio)
  {
    fprintf(stderr, "Erro ao criar a fila de prioridade.\n");
    destroi_mundo(mundo);
    return 1;
  }

  // Insere eventos iniciais na fila de prioridade
  evento_inicio(mundo, fprio);

  // Processa eventos enquanto a fila não estiver vazia ou o tempo acabar
  processa_evento(mundo, fprio);

  // Libera as memorias alocadas
  fprio_destroi(fprio);
  destroi_mundo(mundo);

  return 0;
}