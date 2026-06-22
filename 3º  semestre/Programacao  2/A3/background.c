#include "background.h"
#include "camera.h"
#include "motor_grafico.h"

// Constantes com as velocidades de cada camada
const float DEFAULT_VEL_CEU = 0.1f;
const float DEFAULT_VEL_MONTANHA = 0.3f;
const float DEFAULT_VEL_GREEN = 0.6f;

void iniciar_background(background *bg, ALLEGRO_BITMAP *b1, ALLEGRO_BITMAP *b2, ALLEGRO_BITMAP *b3, float tela_w, float tela_h)
{
    // Atribuindo os ponteiros de imagem
    bg->fundo_ceu = b1;
    bg->fundo_montanha = b2;
    bg->fundo_green = b3;

    // Definindo as velocidades
    bg->vel_ceu = DEFAULT_VEL_CEU;
    bg->vel_montanha = DEFAULT_VEL_MONTANHA;
    bg->vel_green = DEFAULT_VEL_GREEN;

    // Guardando o tamanho da tela
    bg->tela_w = tela_w;
    bg->tela_h = tela_h;

    // Fazendo cache das larguras originais das imagens
    bg->w_ceu = b1 ? (float)al_get_bitmap_width(b1) : 0.0f;
    bg->w_montanha = b2 ? (float)al_get_bitmap_width(b2) : 0.0f;
    bg->w_green = b3 ? (float)al_get_bitmap_width(b3) : 0.0f;
}

void desenhar_background(background *bg, camera *c)
{
    if (!bg->fundo_ceu || !bg->fundo_montanha || !bg->fundo_green || !c)
        return;

    float offset_ceu, offset_montanha, offset_green, h_ceu, h_montanha, h_green, escala_ceu, escala_montanha, escala_green, draw_w_ceu, draw_w_montanha, draw_w_green, pos_x;

    // Variaveis para auxiliar com a altura original
    h_ceu = (float)al_get_bitmap_height(bg->fundo_ceu);
    h_montanha = (float)al_get_bitmap_height(bg->fundo_montanha);
    h_green = (float)al_get_bitmap_height(bg->fundo_green);

    // Calculando a escala para a imagem preencher a altura da tela
    escala_ceu = bg->tela_h / h_ceu;
    escala_montanha= bg->tela_h / h_montanha;
    escala_green = bg->tela_h / h_green;

    // Calculando a nova largura da imagem na tela
    draw_w_ceu = bg->w_ceu * escala_ceu;
    draw_w_montanha = bg->w_montanha * escala_montanha;
    draw_w_green = bg->w_green * escala_green;
    
    // Rolamentos de Background
    // Céu
    float mod_ceu = fmod(c->x * bg->vel_ceu, draw_w_ceu);
    if (mod_ceu < 0) mod_ceu += draw_w_ceu;
    offset_ceu = -mod_ceu;

    // Montanha
    float mod_montanha = fmod(c->x * bg->vel_montanha, draw_w_montanha);
    if (mod_montanha < 0) mod_montanha += draw_w_montanha;
    offset_montanha = -mod_montanha;

    // Colinas Verdes
    float mod_green = fmod(c->x * bg->vel_green, draw_w_green);
    if (mod_green < 0) mod_green += draw_w_green;
    offset_green = -mod_green;

    // Desenha o Céu até cobrir toda a largura da tela
    pos_x = offset_ceu;
    while (pos_x < bg->tela_w)
    {
        al_draw_scaled_bitmap(bg->fundo_ceu, 0, 0, bg->w_ceu, h_ceu, 
                            pos_x, 0, draw_w_ceu, bg->tela_h, 0);
        pos_x += draw_w_ceu; // Avança a posição para o lado
    }

    // Desenha as Montanhas até cobrir toda a largura da tela
    pos_x = offset_montanha;
    while (pos_x < bg->tela_w)
    {
        al_draw_scaled_bitmap(bg->fundo_montanha, 0, 0, bg->w_montanha, h_montanha, 
                            pos_x, 0, draw_w_montanha, bg->tela_h, 0);
        pos_x += draw_w_montanha;
    }

    // Desenha as Colinas Verdes até cobrir toda a largura da tela
    pos_x = offset_green;
    while (pos_x < bg->tela_w)
    {
        al_draw_scaled_bitmap(bg->fundo_green, 0, 0, bg->w_green, h_green, 
                            pos_x, 0, draw_w_green, bg->tela_h, 0);
        pos_x += draw_w_green;
    }
}