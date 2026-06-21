#ifndef MUNDO_H
#define MUNDO_H

#include "armadilhas.h"

// Estrutura para representar uma plataforma no jogo
typedef struct plataforma
{
    float x, y;
    float largura, altura;
    float vel_x, vel_y;
    bool invisivel;
}plataforma;

typedef struct fase
{
    plataforma *plataformas;
    int num_plataformas;
    int capacidade_plataformas;

    armadilha *armadilhas;
    int num_armadilhas;
    int capacidade_armadilhas;
}fase;

// Estrutura para representar a câmera no jogo
typedef struct camera camera;

// Função para carregar um mapa a partir de um arquivo
int carregar_mapa(fase *f, const char *nome_arquivo, ALLEGRO_BITMAP *img_suriken, ALLEGRO_BITMAP *img_espinho, ALLEGRO_BITMAP *img_lanca, ALLEGRO_BITMAP *img_fogo_caixa, ALLEGRO_BITMAP *img_fogo_chama, ALLEGRO_BITMAP *img_bloco);

// Função para iniciar a fase, alocando memória para as plataformas
int iniciar_fase(fase *f);

// Função para adicionar uma plataforma à fase
int adicionar_plataforma(fase *f, float x, float y, float largura, float altura, bool invisivel);

// Atualiza a fase para animar as armadilhas
int atualizar_fase(fase *f);

// Função para atualizar a fase, movendo as plataformas e verificando colisões
int desenhar_fase(fase *f, camera *c);

// Função para destruir a fase, liberando a memória alocada para as plataformas
int destruir_fase(fase *f);

#endif // MUNDO_H