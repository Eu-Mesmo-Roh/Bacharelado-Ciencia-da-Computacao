#include "menu.h"

void iniciar_menu(menu *m)
{
    // Configurando as opções do menu
    m->opcao_selecionada = 0;
    m->total_opcoes = 3;
}

void tratar_imput_menu(menu *m, int tecla, estado_jogo *estado_atual)
{
    // Navega para cima
    if (tecla == ALLEGRO_KEY_W || tecla == ALLEGRO_KEY_UP)
    {
        m->opcao_selecionada--;
        if (m->opcao_selecionada < 0)
            m->opcao_selecionada = m->total_opcoes - 1;
    }
    // Navega para baixo
    else if (tecla == ALLEGRO_KEY_S || tecla == ALLEGRO_KEY_DOWN)
    {
        m->opcao_selecionada++;
        if (m->opcao_selecionada >= m->total_opcoes)
            m->opcao_selecionada = 0;
    }
    // Confirma a seleção
    else if (tecla == ALLEGRO_KEY_ENTER)
    {
        if (m->opcao_selecionada == 0)
            *estado_atual = ESTADO_JOGANDO;
        else if (m->opcao_selecionada == 1)
            *estado_atual = ESTADO_OPCOES;
        else if (m->opcao_selecionada == 2)
            *estado_atual = ESTADO_SAIR;
    }
}

void desenhar_menu(menu *m, ALLEGRO_FONT *fonte)
{
    // Cor de fundo do menu principal
    al_clear_to_color(al_map_rgb(15, 15, 20));

    // Título do Jogo
    al_draw_text(fonte, al_map_rgb(255, 215, 0), 400, 150, ALLEGRO_ALIGN_CENTER, "2D Souls");
    al_draw_line(250, 180, 550, 180, al_map_rgb(100, 100, 100), 2.0f);

    ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200);
    ALLEGRO_COLOR cor_selecionada = al_map_rgb(0, 255, 255);

    // Textos centralizados
    al_draw_text(fonte, m->opcao_selecionada == 0 ? cor_selecionada : cor_normal, 400, 250, ALLEGRO_ALIGN_CENTER, "Jogar");
    al_draw_text(fonte, m->opcao_selecionada == 1 ? cor_selecionada : cor_normal, 400, 300, ALLEGRO_ALIGN_CENTER, "Opcoes");
    al_draw_text(fonte, m->opcao_selecionada == 2 ? cor_selecionada : cor_normal, 400, 350, ALLEGRO_ALIGN_CENTER, "Sair");

    // Lógica do cursor exata
    float cursor_x = 330; // Posicionado à esquerda das palavras
    float cursor_y = 250;

    if (m->opcao_selecionada == 0)      cursor_y = 250;
    else if (m->opcao_selecionada == 1) cursor_y = 300;
    else if (m->opcao_selecionada == 2) cursor_y = 350;

    al_draw_text(fonte, cor_selecionada, cursor_x, cursor_y, ALLEGRO_ALIGN_LEFT, ">");
    
    // Pequeno texto de explicativo
    al_draw_text(fonte, al_map_rgb(100, 100, 100), 400, 550, ALLEGRO_ALIGN_CENTER, "Use W/S para navegar e ENTER para selecionar");
}

