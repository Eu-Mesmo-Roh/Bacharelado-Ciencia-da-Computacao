#ifndef ARMADILHAS_H
#define ARMADILHAS_H

#include "motor_grafico.h"
#include "camera.h"

typedef enum
{
    armadilha_suriken = 1,
    armadilha_espinho_estatico = 2,
    armadilha_lanca_retratil = 3,
    armadilha_fogo_temporizado = 4,
    armadilha_bloco_movel = 5
}tipo_armadilha;

typedef struct armadilha
{
    float x, y;
    float largura, altura;
    float offset_x, offset_y;

    tipo_armadilha tipo;

    // Controle de Animação
    ALLEGRO_BITMAP *sprite;
    int frame_atual;
    int max_frames;
    int timer_animacao;
    int velocidade_animacao;
    int largura_frame;

    // Controle exclusivo do lança-chamas
    ALLEGRO_BITMAP *sprite_secundario;
    int frame_secundario;

    // Controles de tempo, Estados e física das armadilhas
    int timer_estado;
    int estado_ativo;
    float vel_x;

    // Variaveis para o bloco movel
    float x_inicial;
    float limite_patrulha;
}armadilha;

// Função que cria e inicializa a armadilha
void iniciar_armadilha(armadilha *a, float x, float y, float largura, float altura, tipo_armadilha tipo, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img_secundaria, int max_frames, int largura_frame);

// Função para atualizar a lógica da armadilha, como animaçãoção e movimento
void atualizar_armadilha(armadilha *a);

// Função para desenhar a armadilha na tela, considerando a posição da câmera
void desenhar_armadilha(armadilha *a, camera *c);

#endif // ARMADILHAS_H