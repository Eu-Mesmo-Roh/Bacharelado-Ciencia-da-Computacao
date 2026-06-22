#include "camera.h"
#include "player.h"

int iniciar_camera(camera *c, float largura_tela, float altura_tela)
{
    c->x = 0.0f;
    c->y = 0.0f;
    c->largura_tela = largura_tela;
    c->altura_tela = altura_tela;

    return 1; // Sucesso
}

int atualizar_camera(camera *c, player *p)
{
    float pe_do_player;

    // Centralizando a câmera na posição do player
    c->x = p->x + (p->largura / 2.0f) - (c->largura_tela / 2.0f);
    pe_do_player = p->y + p->altura;
    c->y = pe_do_player - (p->altura_original / 2.0f) - (c->altura_tela / 2.0f);

    // Garantindo que a câmera não mostre áreas fora do mundo do jogo
    if (c->x < 0)
        c->x = 0;
    if (c->y > 50)
        c->y = 50;

    return 1; // Sucesso
}