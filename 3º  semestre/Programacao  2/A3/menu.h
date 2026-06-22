#ifndef MENU_H
#define MENU_H

#include "motor_grafico.h"

// Estrutura para representar os diversos estados do jogo
typedef enum
{
    ESTADO_MENU_PRINCIPAL,
    ESTADO_JOGANDO,
    ESTADO_PAUSADO,
    ESTADO_OPCOES,
    ESTADO_GAME_OVER,
    ESTADO_VITORIA,
    ESTADO_SAIR
} estado_jogo;

// Estrutura de opções do menu
typedef struct menu
{
    // 0 = Jogar, 1 = Opções, 2 = Sair
    int opcao_selecionada;
    int total_opcoes;
} menu;

// Função que inicializa o menu do jogo
void iniciar_menu(menu *m);

// Função que trata as ações no menu
void tratar_imput_menu(menu *m, int tecla, estado_jogo *estado_atual);

// Função que desenha o menu principal
void desenhar_menu(menu *m, ALLEGRO_FONT *fonte);

#endif // MENU_H
