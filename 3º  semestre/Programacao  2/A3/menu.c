#include "menu.h"

int iniciar_menu(menu *m)
{
    // Configurando as opções do menu
    m->opcao_selecionada = 0;
    m->total_opcoes = 3;
    m->ativo = true;

    return 1; // Sucesso
}

int tratar_imput_menu(menu *m, int tecla, estado_jogo *estado_atual)
{
    if (tecla == ALLEGRO_KEY_DOWN)
        m->opcao_selecionada = (m->opcao_selecionada +1) % m->total_opcoes;
    else if (tecla == ALLEGRO_KEY_UP)
        m->opcao_selecionada = (m->opcao_selecionada -1 + m->total_opcoes) % m->total_opcoes;
    else if (tecla == ALLEGRO_KEY_ENTER)
    {
        if (m->opcao_selecionada == 0)
            *estado_atual = ESTADO_JOGANDO;
        else if (m->opcao_selecionada == 1)
            *estado_atual = ESTADO_OPCOES;
        else if (m->opcao_selecionada == 2)
            *estado_atual = ESTADO_SAIR;
    }

    return 1; // Sucesso
}

int desenhar_menu(menu *m, ALLEGRO_FONT *fonte)
{
    // Desenhando o menu
    al_clear_to_color(al_map_rgb(20, 20, 50));

    // Definido cores para as opções do menu
    ALLEGRO_COLOR cor_normal = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 255, 0);

    // Desenhando o titulo do jogo
    al_draw_text(fonte, al_map_rgb(0, 255, 255), 400, 100, ALLEGRO_ALIGN_CENTER, "2D Souls");
    
    /*-----Desenhando as opções do menu-----*/

    // A opção selecionada é desenhada com uma cor diferente para destacar
    al_draw_text(fonte, (m->opcao_selecionada == 0) ? cor_selecionada : cor_normal, 400, 250, ALLEGRO_ALIGN_CENTER, "Jogar");

    al_draw_text(fonte, (m->opcao_selecionada == 1) ? cor_selecionada : cor_normal, 400, 320, ALLEGRO_ALIGN_CENTER, "Opções");

    al_draw_text(fonte, (m->opcao_selecionada == 2) ? cor_selecionada : cor_normal, 400, 390, ALLEGRO_ALIGN_CENTER, "Sair");

    return 1; // Sucesso
}

