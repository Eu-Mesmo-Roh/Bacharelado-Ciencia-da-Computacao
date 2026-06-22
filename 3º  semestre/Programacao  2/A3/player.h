#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "motor_grafico.h"



// Estrutura para representar o player no jogo
typedef struct player
{
    // Posição do player
    float x, y;
    float altura, largura;
    float vel_x, vel_y;
    bool no_chao;
    bool movendo_para_direita, movendo_para_esquerda;
    bool virado_para_esquerda;

    // Vida do player
    int hp;
    int max_hp;
    int timer_invuleravel;
    int timer_knockback;
    bool tomando_dano;

    /* Outras interações do player */
    // Variável para indicar se o player está abaixado
    bool abaixado;
    float altura_original;

    // Variavel para indicar o desvio do player para a direita ou esquerda
    bool rolamento;
    int tempo_rolamento;

    // Sprite do player
    ALLEGRO_BITMAP *sprite_parado;
    ALLEGRO_BITMAP *sprite_correndo;
    ALLEGRO_BITMAP *sprite_parado_abaixado;
    ALLEGRO_BITMAP *sprite_andando_abaixado;
    ALLEGRO_BITMAP *sprite_rolando;
    ALLEGRO_BITMAP *sprite_inverter;
    ALLEGRO_BITMAP *sprite_pular;
    ALLEGRO_BITMAP *sprite_cair;
    ALLEGRO_BITMAP *sprite_atual;
    ALLEGRO_BITMAP *sprite_dano;
    ALLEGRO_BITMAP *sprite_morte;


    int frame_atual;
    int timer_animacao;
    int jump_buffer;
    int coyote_time;
    bool morto;

}player;

// Estrutura para representar a câmera no jogo
typedef struct camera camera;

// Estrutura para representar a fase do jogo
typedef struct fase fase;

// Função para iniciar o player, definindo suas propriedades iniciais
int iniciar_player(player *p, float x, float y, float altura, float largura, ALLEGRO_BITMAP *img_parado, ALLEGRO_BITMAP *img_correndo, ALLEGRO_BITMAP *img_parado_abaixado, ALLEGRO_BITMAP *img_andando_abaixado, ALLEGRO_BITMAP *img_rolando, ALLEGRO_BITMAP *img_inverter, ALLEGRO_BITMAP *img_pular, ALLEGRO_BITMAP *img_cair, ALLEGRO_BITMAP *img_dano, ALLEGRO_BITMAP *img_morte);

// Função para atualizar a posição do player, aplicando a gravidade e verificando colisões
int atualizar_player(player *p, fase *f);

// Função para fazer o player pular
int pular_player(player *p);

// Função para desenhar o player na tela usando o motor gráfico Allegro
int desenhar_player(player *p, camera *c);

// Função que da dano no player
int dar_dano_player(player *p, float x_dano);

#endif // PLAYER_H
