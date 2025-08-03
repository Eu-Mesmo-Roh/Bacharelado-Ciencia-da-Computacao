#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "fprio.h"
#include "lista.h"

// Defines
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10

// Descreve um local 
struct local_t
{
    int x; // cordenadas x
    int y; // cordenadas y
};


// Descreve um Heroi
struct heroi_t
{
    int id;          // número inteiro ≥ 0 que identifica unicamente o herói
    struct cjto_t *habilidades; // conjunto de habilidades que o herói possui
    int paciencia;   // número inteiro ≥ 0 que indica quão paciente um Heroi é
    int velocidade;  // número inteiro ≥ 0 indicando a velocidade de deslocamento de um herói
    int experiencia; // número inteiro ≥ 0 que indica o número de missões em que o herói já participou
    int base;        // ID da base onde o herói se encontra no momento
    int status;      // verifica se o heroi esta vivo ou morto
};

// Descreve a base dos Herois
struct base_t
{
    int id;                   // número inteiro ≥ 0 que identifica cada base
    int lotacao;              // número máximo de heróis na base
    int fila_max;             //  tamanho maximo que a fila chegou 
    int qtd_missoes;          //  quantidade de missões concluidas pela base 
    struct cjto_t *presentes; // conjunto que constitue quais herois estão atualmente na base
    struct lista_t *espera;     // fila de espera para poder entrar na base
    struct local_t *coordenadas;             // localização da base

};

// Descreve uma misão
struct missao_t
{
    int id;                    // número inteiro ≥ 0 que identifica a missão
    struct cjto_t *habilidades; // conjunto de habilidades necessárias para cumprir a missão
    int realizadas;             // caso a missao tenha sido concluida ou não
    int perigo;                // nível de perigo da missão
    int tentativas;            // número de tentativas de cumprir a missão
    struct local_t *coordenadas;              // localização da missão
};

// Descreve um mundo
struct mundo_t
{   
    struct heroi_t *herois;    // vetor representando todos os heróis
    struct base_t *bases;      // vetor representando todas as bases
    struct missao_t *missoes; // vetor representando todas as missões
    int NHerois;             // número total de heróis no mundo
    int NBases;              // número total de bases no mundo
    int NMissoes;            // número total de missões a cumprir
    int NHabilidades;        // número de habilidades distintas possíveis
    int tamanhoMundo;        // coordenadas máximas do mundo
    int tempo;             // número inteiro ≥ 0 indicando o tempo atual no mundo.
    int eventos_tratados;   // eventos tratados durante a execução
};

// Retorna um inteiro aleatório
int gera_valor_aleatorio(int min, int max);

// cria um heroi
// Retorno: ponteiro para o heroi criado ou NULL se erro
struct heroi_t *cria_heroi(struct mundo_t *mundo);

// Libera todas as estruturas de dados do heroi
// Retorno: NULL
void destroi_heroi(struct mundo_t *mundo);

// cria uma base
// Retorno: ponteiro para a base criada ou NULL se erro
struct base_t *cria_base(struct mundo_t *mundo);

// Libera todas as estruturas de dados da base
// Retorno: NULL
void destroi_base(struct mundo_t *mundo);

// cria uma missão
// Retorno: ponteiro para a missão criada ou NULL se erro
struct missao_t *cria_missao(struct mundo_t *mundo);

// Libera todas as estruturas de dados da missão
// Retorno: NULL
void destroi_missao(struct mundo_t *mundo);

// cria um mundo
// Retorno: ponteiro para o mundo criado ou NULL se erro
struct mundo_t *cria_mundo();

// Libera todas as estruturas de dados do mundo
// Retorno: NULL
void destroi_mundo(struct mundo_t *mundo);

#endif