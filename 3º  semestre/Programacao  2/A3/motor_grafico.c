#include <motor_grafico.h>

int init_motor_grafico()
{
    //iniciando o Allegro
    if(!al_init())
    {
        fprintf(stderr, "Falha ao Inicializar Allegro!\n");
        return -1;
    }

    //iniciando o modulo de imagem
    if(!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao Inicializar o modulo de imagem!\n");
        return -1;
    }

    //iniciando o modulo de fontes
    if(!al_init_font_addon())
    {
        fprintf(stderr, "Falha ao Inicializar o modulo de fontes!\n");
        return -1;
    }

    //iniciando o modulo de fontes TTF
    {
        if(!al_init_ttf_addon())
        {
            fprintf(stderr, "Falha ao Inicializar o modulo de fontes TTF!\n");
            return -1;
        }
    }
}