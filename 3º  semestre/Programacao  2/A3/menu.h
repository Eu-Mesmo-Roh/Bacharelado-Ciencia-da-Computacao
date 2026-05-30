#ifndef MENU_H
#define MENU_H

#include "motor_grafico.h"

typedef enum
{
    ESTADO_MENU_PRINCIPAL,
    ESTADO_JOGANDO,
    ESTADO_OPCOES,
    ESTADO_GAME_OVER,
    ESTADO_VITORIA
} estado_jogo;

typedef struct menu
{
    // 0 = Jogar, 1 = Opções, 2 = Sair
    int opcao_selecionada;
    int total_opcoes;
    bool ativo;
} menu;

int iniciar_menu(menu *m);

int lidar_imput_menu(menu *m, int tecla, estado_jogo *estado_atual);

int desenhar_menu(menu *m);

#endif // MENU_H
