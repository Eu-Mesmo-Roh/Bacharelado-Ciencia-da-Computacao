#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "motor_grafico.h"

typedef struct camera camera;

// Estrutura para criar o background da fase
typedef struct background
{
    // Sprites de background
    ALLEGRO_BITMAP *fundo_ceu;
    ALLEGRO_BITMAP *fundo_montanha;
    ALLEGRO_BITMAP *fundo_green;

    // Variaveis para configurar as sprites do background
    float vel_ceu, vel_montanha, vel_green;
    float w_ceu, w_montanha, w_green;
    float tela_w, tela_h;
}background;

// Função que inicializa o background com imagens carregadas e dimensões da tela
void iniciar_background(background *bg, ALLEGRO_BITMAP *b1, ALLEGRO_BITMAP *b2, ALLEGRO_BITMAP *b3, float tela_w, float tela_h);

// Função que desenha todas as camadas do fundo em Parallax
void desenhar_background(background *bg, camera *c);


#endif // BACKGROUND_H