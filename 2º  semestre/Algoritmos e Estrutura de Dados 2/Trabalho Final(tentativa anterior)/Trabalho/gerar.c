#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() 
{
  int i;
  FILE *arquivo;

  // Abre o arquivo para escrita
  arquivo = fopen("numeros_aleatorios.txt", "w");

  // Verifica se o arquivo foi aberto corretamente
  if (arquivo == NULL) 
  {
    printf("Erro ao abrir o arquivo!\n");
    return 1;
  }

  // Define a semente para a função rand()
  srand(time(NULL));

  // Gera e grava 50.000 números aleatórios no arquivo, um por linha
  for (i = 0; i < 50000; i++) 
    fprintf(arquivo, "%d\n", rand());

  // Fecha o arquivo após escrever todos os números
  fclose(arquivo);

  printf("Arquivo 'numeros_aleatorios.txt' criado com sucesso.\n");

  return 0;
}