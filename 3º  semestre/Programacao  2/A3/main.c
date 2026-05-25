#include "motor_grafico.h"

int main()
{
    // Iniciando o motor gráfico Allegro
    if (!init_motor_grafico())
    {
        fprintf(stderr, "Erro ao inicializar o motor gráfico!\n");
        return -1;
    }

    // Criando a janela do jogo
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display)
    {
        fprintf(stderr, "Falha ao criar a janela do jogo!\n");
        return -1;
    }

    // Configurando o título da janela
    al_set_window_title(display, "Darknight");

    // Cor de fundo da janela
    al_clear_to_color(al_map_rgb(0, 0, 0));
    // Exibindo a janela
    al_flip_display();

    // Aguarda algum tempo antes de fechar a janela
    al_rest(5.0);

    // Limpando os recursos do Allegro
    al_destroy_display(display);
    return 0;
}