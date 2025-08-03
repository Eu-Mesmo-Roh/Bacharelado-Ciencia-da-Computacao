#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "entidades.h"

// Função que gera um valor aleatório entre min e max
int gera_valor_aleatorio(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

// Função que cria um vetor de heróis dentro de um mundo
struct heroi_t *cria_heroi(struct mundo_t *mundo)
{
    struct heroi_t *novo_heroi;
    int i;

    novo_heroi = malloc(sizeof(struct heroi_t) * mundo->NHerois);

    // Inicializa os atributos de cada herói no vetor
    if (!novo_heroi)
        return NULL;

    for (i = 0; i < mundo->NHerois; i++)
    {
        novo_heroi[i].id = i;
        novo_heroi[i].experiencia = 0;
        novo_heroi[i].paciencia = gera_valor_aleatorio(0, 100);
        novo_heroi[i].velocidade = gera_valor_aleatorio(50, 5000);
        novo_heroi[i].habilidades = cjto_aleat(gera_valor_aleatorio(1, 3), 10);
        novo_heroi[i].base = -10;
        novo_heroi[i].status = 1;
    }

    return novo_heroi;
}

// Função que libera os recursos alocados para os heróis no mundo
void destroi_heroi(struct mundo_t *mundo)
{
    int i;

    if (!mundo || !mundo->herois)
        return;

    // Libera os conjuntos de habilidades de cada herói
    for (i = 0; i < mundo->NHerois; i++)
    {
        if (mundo->herois[i].habilidades)
            cjto_destroi(mundo->herois[i].habilidades);
    }

    free(mundo->herois);
    mundo->herois = NULL;
}

// Função que cria um vetor de bases dentro de um mundo
struct base_t *cria_base(struct mundo_t *mundo)
{
    struct base_t *nova_base;
    int i;

    if (!mundo)
        return NULL;

    nova_base = malloc(sizeof(struct base_t) * mundo->NBases);
    if (!nova_base)
        return NULL;

    // Inicializa os atributos de cada base
    for (i = 0; i < mundo->NBases; i++)
    {
        nova_base[i].id = i;
        nova_base[i].lotacao = gera_valor_aleatorio(3, 10);
        nova_base[i].presentes = cjto_cria(nova_base[i].lotacao);
        nova_base[i].fila_max = 0;
        nova_base[i].espera = lista_cria();
        nova_base[i].qtd_missoes = 0;

        // Aloca e define as coordenadas da base
        nova_base[i].coordenadas = malloc(sizeof(struct local_t));
        if (!nova_base[i].coordenadas)
            return NULL;

        nova_base[i].coordenadas->x = gera_valor_aleatorio(0, N_TAMANHO_MUNDO - 1);
        nova_base[i].coordenadas->y = gera_valor_aleatorio(0, N_TAMANHO_MUNDO - 1);
    }
    return nova_base;
}

// Função que libera os recursos alocados para as bases no mundo
void destroi_base(struct mundo_t *mundo)
{
    int i;

    if (!mundo || !mundo->bases)
        return;

    // Libera os recursos de cada base
    for (i = 0; i < mundo->NBases; i++)
    {
        cjto_destroi(mundo->bases[i].presentes);
        lista_destroi(mundo->bases[i].espera);
        free(mundo->bases[i].coordenadas);
    }

    free(mundo->bases);
    mundo->bases = NULL;
}

// Função que cria um vetor de missões dentro de um mundo
struct missao_t *cria_missao(struct mundo_t *mundo)
{
    struct missao_t *nova_missao;
    int i;

    nova_missao = malloc(sizeof(struct missao_t) * mundo->NMissoes);
    if (!nova_missao)
        return NULL;

    // Inicializa os atributos de cada missão
    for (i = 0; i < mundo->NMissoes; i++)
    {
        nova_missao[i].id = i;
        nova_missao[i].habilidades = cjto_aleat(gera_valor_aleatorio(6, 10), 10);
        nova_missao[i].perigo = gera_valor_aleatorio(0, 100);
        nova_missao[i].realizadas = 0;
        nova_missao[i].tentativas = 0;

        // Aloca e define as coordenadas da missão
        nova_missao[i].coordenadas = malloc(sizeof(struct local_t));
        if (!nova_missao[i].coordenadas)
            return NULL;

        nova_missao[i].coordenadas->x = gera_valor_aleatorio(0, N_TAMANHO_MUNDO - 1);
        nova_missao[i].coordenadas->y = gera_valor_aleatorio(0, N_TAMANHO_MUNDO - 1);
    }

    return nova_missao;
}

// Função que libera os recursos alocados para as missões no mundo
void destroi_missao(struct mundo_t *mundo)
{
    int i;

    if (!mundo || !mundo->missoes)
        return;

    // Libera os recursos de cada missão
    for (i = 0; i < mundo->NMissoes; i++)
    {
        cjto_destroi(mundo->missoes[i].habilidades);
        free(mundo->missoes[i].coordenadas);
    }

    free(mundo->missoes);
    mundo->missoes = NULL;
}

// Função que cria e inicializa um mundo
struct mundo_t *cria_mundo()
{
    struct mundo_t *novo_mundo;
    int N_herois, N_bases, N_missoes;

    /* aloca a memoria do mundo */
    novo_mundo = malloc(sizeof(struct mundo_t));
    if (!novo_mundo)
        return NULL;

    N_herois = N_HABILIDADES * 5;
    N_bases = N_herois / 5;
    N_missoes = T_FIM_DO_MUNDO / 100;

    /* Inicializa parametros */
    novo_mundo->tamanhoMundo = N_TAMANHO_MUNDO;
    novo_mundo->NHabilidades = N_HABILIDADES;
    novo_mundo->NHerois = N_herois;
    novo_mundo->herois = cria_heroi(novo_mundo);
    novo_mundo->NBases = N_bases;
    novo_mundo->bases = cria_base(novo_mundo);
    novo_mundo->NMissoes = N_missoes;
    novo_mundo->missoes = cria_missao(novo_mundo);
    novo_mundo->tempo = T_INICIO;
    novo_mundo->eventos_tratados = 0;

    return novo_mundo;
}

// Função que libera todos os recursos alocados para o mundo
void destroi_mundo(struct mundo_t *mundo)
{
    if (!mundo)
        return;

    // Libera as missões, bases e heróis
    destroi_missao(mundo);
    destroi_base(mundo);
    destroi_heroi(mundo);

    // Libera a estrutura do mundo e define o ponteiro como NULL
    free(mundo);
    mundo = NULL;
}
