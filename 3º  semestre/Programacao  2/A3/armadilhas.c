#include "armadilhas.h"

void iniciar_armadilha(armadilha *a, float x, float y, float largura, float altura, tipo_armadilha tipo, ALLEGRO_BITMAP *img, int max_frames, int largura_frame)
{
    a->x = x;
    a->y = y;
    a->largura = largura;
    a->altura = altura;
    a->tipo = tipo;
    a->sprite = img;
    a->frame_atual = 0;
    a->timer_animacao = 0;
    a->max_frames = max_frames;
    a->velocidade_animacao = 3; // Velocidade de animação padrão
    a->largura_frame = largura_frame;
}

void atualizar_armadilha(armadilha *a)
{
    if (a->sprite && a->max_frames > 1)
    {
        a->timer_animacao++;
        if (a->timer_animacao >= a->velocidade_animacao)
        {
            a->timer_animacao = 0;
            a->frame_atual++;
            if (a->frame_atual >= a->max_frames)
                a->frame_atual = 0;
        }
    }
}

void desenhar_armadilha(armadilha *a, camera *c)
{
    if (a->sprite)
    {
        int pos_x_recorte = a->frame_atual * a->largura_frame;
        float escala = 2.0f; // ajustar conforme a arte

        // hitbox para teste
        al_draw_rectangle(a->x - c->x, a->y - c->y, a->x + a->largura - c->x, a->y + a->altura - c->y, al_map_rgb(255, 255, 0), 2.0f);

        al_draw_scaled_bitmap(a->sprite, pos_x_recorte, 0, a->largura_frame, al_get_bitmap_height(a->sprite), (a->x - c->x), (a->y - c->y), a->largura_frame * escala, al_get_bitmap_height(a->sprite) * escala, 0);
    }
}
