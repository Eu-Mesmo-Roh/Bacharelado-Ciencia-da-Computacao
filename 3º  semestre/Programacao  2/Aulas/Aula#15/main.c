#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

int main()
{   
    // Inicialização do Allegro
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();

    // Configuração do modo gráfico
    ALLEGRO_DISPLAY *display = al_create_display(640,480);

    // Cria uma fonte para desenhar o texto
    ALLEGRO_FONT *font = al_create_builtin_font();

    // Carrega a imagem usando a função al_load_bitmap, passando o nome do arquivo e a paleta
    ALLEGRO_BITMAP *img = al_load_bitmap("teste.jpg");

    // Estrutura para capturar eventos do teclado
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    bool rodando = true;

    while (rodando)
    {
        ALLEGRO_EVENT event;
        //verifica se houve alguma entrada de teclado
        while (al_get_next_event(event_queue, &event))
        {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = false;
            }
        }
        
        // Limpa a tela com a cor preta
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Desenha o texto na tela usando a fonte criada
        al_draw_text(font, al_map_rgb(255, 0, 0), 30, 20, 0, "Exemplo de imagem");

        al_draw_bitmap(img,30, 50, 0);

        al_flip_display();
    }

    return 0;
}