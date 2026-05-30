#include "motor_grafico.h"

int init_motor_grafico()
{
    //iniciando o Allegro
    if (!al_init())
    {
        fprintf(stderr, "Falha ao Inicializar Allegro!\n");
        return 0;
    }

    //iniciando o modulo de imagem
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao Inicializar o modulo de imagem!\n");
        return 0;
    }

    //iniciando o modulo de fontes
    if (!al_init_font_addon())
    {
        fprintf(stderr, "Falha ao Inicializar o modulo de fontes!\n");
        return 0;
    }

    //iniciando o modulo de fontes TTF
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao Inicializar o modulo de fontes TTF!\n");
        return 0;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao Inicializar o modulo de primitivas!\n");
        return 0;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao Inicializar o teclado!\n");
        return 0;
    }

    return 1;
}