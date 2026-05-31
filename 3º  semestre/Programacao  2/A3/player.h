#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>


// Estrutura para representar o player no jogo
typedef struct player
{
    // Posição do player
    float x, y;
    float altura, largura;
    float vel_x, vel_y;
    bool no_chao;
    bool movendo_para_direita, movendo_para_esquerda;

    // Vida do player
    int hp;
    int max_hp;

    /* Outras interações do player */
    // Variável para indicar se o player está abaixado
    bool abaixado;
    float altura_original;

    // Variavel para indicar o desvio do player para a direita ou esquerda
    bool rolamento;
    int tempo_rolamento;

}player;

// Estrutura para representar a câmera no jogo
typedef struct camera camera;

// Estrutura para representar a fase do jogo
typedef struct fase fase;

// Função para iniciar o player, definindo suas propriedades iniciais
int iniciar_player(player *p, float x, float y, float altura, float largura);

// Função para atualizar a posição do player, aplicando a gravidade e verificando colisões
int atualizar_player(player *p, fase *f);

// Função para fazer o player pular
int pular_player(player *p);

// Função para desenhar o player na tela usando o motor gráfico Allegro
int desenhar_player(player *p, camera *c);

#endif // PLAYER_H
