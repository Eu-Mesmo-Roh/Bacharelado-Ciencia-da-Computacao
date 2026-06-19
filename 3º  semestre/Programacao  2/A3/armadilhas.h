#ifndef ARMADILHAS_H
#define ARMADILHAS_H

#include "motor_grafico.h"
#include "camera.h"

typedef enum
{
    armadilha_shuriken,
    armadilha_espinho,
    armadilha_fogo
}tipo_armadilha;

typedef struct armadilha
{
    float x, y;
    float largura, altura;

    tipo_armadilha tipo;

    ALLEGRO_BITMAP *sprite;
    int frame_atual;
    int max_frames;
    int timer_animacao;
    int velocidade_animacao;
    int largura_frame;
}armadilha;

// Função que cria e inicializa a armadilha
void iniciar_armadilha(armadilha *a, float x, float y, float largura, float altura, tipo_armadilha tipo, ALLEGRO_BITMAP *img, int max_frames, int largura_frame);

// Função para atualizar a lógica da armadilha, como animaçãoção e movimento
void atualizar_armadilha(armadilha *a);

// Função para desenhar a armadilha na tela, considerando a posição da câmera
void desenhar_armadilha(armadilha *a, camera *c);

#endif // ARMADILHAS_H