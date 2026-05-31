#include "motor_grafico.h"
#include "player.h"
#include "mundo.h"
#include "camera.h"
#include "menu.h"

const float FPS = 60.0;

int main()
{
    // Iniciando o motor gráfico Allegro
    if (!init_motor_grafico())
    {
        fprintf(stderr, "Erro ao inicializar o motor gráfico!\n");
        return -1;
    }

    ALLEGRO_FONT *fonte = al_create_builtin_font();
    if (!fonte)
    {
        fprintf(stderr, "Erro ao carregar a fonte!\n");
        return -1;
    }

    // Criando o timer, fila de eventos e display
    ALLEGRO_TIMER *tempo = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);

    if (!tempo || !fila_eventos || !display)
    {
        fprintf(stderr, "Falha ao criar os componentes do jogo!\n");
        return -1;
    }

    // Registrando os eventos
    al_register_event_source (fila_eventos, al_get_keyboard_event_source());
    al_register_event_source (fila_eventos, al_get_display_event_source(display));
    al_register_event_source (fila_eventos, al_get_timer_event_source(tempo));

    // Variaveis para o controle do jogo
    bool rodando = true;
    bool desenhar = true;

    // Iniciando o timer
    al_start_timer (tempo);

    // Estado do jogo
    estado_jogo estado_atual = ESTADO_MENU_PRINCIPAL;

    //criando o menu
    menu menu_principal;
    iniciar_menu(&menu_principal);
    
    // Criando o player
    player jogador;
    iniciar_player(&jogador, 100, 100, 50, 30);
    
    // Criando o nível
    fase nivel_1;
    iniciar_fase(&nivel_1);
    if (!carregar_mapa(&nivel_1, "fase1.csv"))
    {
        fprintf(stderr, "Erro ao carregar o mapa!\n");
        destruir_fase(&nivel_1);

        al_destroy_display(display);
        al_destroy_timer(tempo);
        al_destroy_event_queue(fila_eventos);

        return -1;
    }
    
    // Criando a câmera
    camera cam;
    iniciar_camera(&cam, 800, 600);

    // Loop principal do jogo
    while (rodando)
    {
        ALLEGRO_EVENT evento;

        // Esperando por um evento
        al_wait_for_event(fila_eventos, &evento);

        /* ----- Processando os eventos ----- */

        if (estado_atual == ESTADO_SAIR)
        {
            rodando = false;
            continue;
        }

        // Se o usuario fechou a janela, o jogo deve ser encerrado
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            rodando = false;

        // Se o evento inicia o tempo, a lógica do jogo deve ser atualizada
        else if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            if (estado_atual == ESTADO_JOGANDO)
            {
                if (jogador.rolamento)
                {
                    jogador.tempo_rolamento--;

                    if (jogador.tempo_rolamento <= 0)
                    {
                        jogador.rolamento = false;

                        // Atualizando o estado do teclado para garantir que o jogador não fique preso em um estado de movimento após o rolamento
                        ALLEGRO_KEYBOARD_STATE estado_teclado;
                        al_get_keyboard_state(&estado_teclado);

                        // Verificando se o jogador ainda está segurando a tecla de agachar para decidir se ele deve permanecer abaixado ou voltar à posição normal
                        bool segurando_agachar = al_key_down(&estado_teclado, ALLEGRO_KEY_S);

                        // Se o jogador não está segurando a tecla de agachar, ele volta à posição normal
                        if (!segurando_agachar)
                        {
                            if (jogador.abaixado)
                            {
                                jogador.abaixado = false;
                                jogador.y -= jogador.altura_original / 2.0f; 
                                jogador.altura = jogador.altura_original;
                            }
                        }

                        bool segurando_pulo = al_key_down(&estado_teclado, ALLEGRO_KEY_W) || al_key_down(&estado_teclado, ALLEGRO_KEY_SPACE);

                        if (segurando_pulo && jogador.no_chao)
                        {
                            pular_player(&jogador);
                        }
                    }
                }

                // Aqui é onde a lógica do jogo será atualizada
                atualizar_player(&jogador, &nivel_1);
                atualizar_camera(&cam, &jogador);

                if (jogador.no_chao && !jogador.rolamento)
                {
                    ALLEGRO_KEYBOARD_STATE estado_teclado;
                    al_get_keyboard_state(&estado_teclado);

                    if (al_key_down(&estado_teclado, ALLEGRO_KEY_S) && !jogador.abaixado)
                    {
                        jogador.abaixado = true;

                        // Reduz a altura do player pela metade
                        jogador.altura = jogador.altura_original / 2.0f;
                        // Ajusta a posição y para que o player pareça estar se abaixando
                        jogador.y += jogador.altura_original / 2.0f;
                    }
                }

                // Se o jogador cair da tela, ele volta para a posição inicial
                if (jogador.y > 1000.0f) 
                    jogador.hp = 0;
                
                if (jogador.hp <= 0)
                    estado_atual = ESTADO_GAME_OVER;
                
                if (jogador.x > 2500.0f) 
                    estado_atual = ESTADO_VITORIA;
                
            }

            desenhar = true;
        }

        /* ----- Se uma tecla foi precionada ----- */
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (estado_atual == ESTADO_MENU_PRINCIPAL)
                tratar_imput_menu(&menu_principal, evento.keyboard.keycode, &estado_atual);
            
            else if (estado_atual == ESTADO_JOGANDO)
            {
                if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    rodando = false;
            
                // Jogador começa a se mover para a direita ou esquerda dependendo da tecla precionada
                if (evento.keyboard.keycode == ALLEGRO_KEY_D)
                    jogador.movendo_para_direita = true;
                else if (evento.keyboard.keycode == ALLEGRO_KEY_A)
                    jogador.movendo_para_esquerda = true;
                
                // Jogador pula se a barra de espaço for precionada
                if (evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
                    pular_player(&jogador);

                // Para fim de teste o jogador leva dano
                if (evento.keyboard.keycode == ALLEGRO_KEY_K)
                {
                    jogador.hp -= 1;
                    printf("O jogador tomou dano. Vida restante: %d\n", jogador.hp);
                }

                // O jogador se abaixa se a tecla S for precionada, e volta à posição normal quando a tecla for solta
                if (evento.keyboard.keycode == ALLEGRO_KEY_S)
                {
                    if (!jogador.abaixado && !jogador.rolamento && jogador.no_chao)
                    {
                        jogador.abaixado = true;

                        // Reduz a altura do player pela metade
                        jogador.altura = jogador.altura_original / 2.0f;

                        // Ajusta a posição y para que o player pareça estar se abaixando
                        jogador.y += jogador.altura_original / 2.0f; 
                    }
                }

                if (evento.keyboard.keycode == ALLEGRO_KEY_LSHIFT)
                {
                    if (!jogador.rolamento && jogador.no_chao && (jogador.movendo_para_direita || jogador.movendo_para_esquerda))
                    {
                        jogador.rolamento = true;
                        jogador.tempo_rolamento = 20; // O rolamento dura 30 frames (meio segundo a 60 FPS)

                        if (jogador.movendo_para_esquerda)
                            jogador.vel_x = -15.0f;
                        else if (jogador.movendo_para_direita)
                            jogador.vel_x = 15.0f;

                        if (!jogador.abaixado)
                        {
                            jogador.abaixado = true;

                            // Reduz a altura do player para simular o rolamento
                            jogador.altura = jogador.altura_original / 2.0f;

                            // Ajusta a posição y para que o player pareça estar rolando
                            jogador.y += jogador.altura_original / 2.0f; 
                        }
                    }
                }
            }

            else if (estado_atual == ESTADO_GAME_OVER)
            {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    // Reiniciando o jogo
                    iniciar_player(&jogador, 100, 100, 50, 30);

                    estado_atual = ESTADO_JOGANDO;
                }
            }

            else if (estado_atual == ESTADO_VITORIA)
            {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    // Reinicia a posição e velocidade para a proxima vez que for jogar
                    iniciar_player(&jogador, 100, 100, 50, 30);

                    // Voltando para o menu
                    estado_atual = ESTADO_MENU_PRINCIPAL;
                }
            }
        }

        /* ----- Se uma tecla foi solta ----- */
        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (estado_atual == ESTADO_JOGANDO)
            {
                if (evento.keyboard.keycode == ALLEGRO_KEY_S)
                {
                    if (jogador.abaixado)
                    {
                        jogador.abaixado = false;

                        // Ajusta a posição y para que o player pareça estar se levantando
                        jogador.y -= jogador.altura_original / 2.0f; 

                        // Restaura a altura original do player
                        jogador.altura = jogador.altura_original;
                    }
                }
                // Jogador para de se mover para a direita ou esquerda dependendo da tecla solta
                if (evento.keyboard.keycode == ALLEGRO_KEY_D)
                    jogador.movendo_para_direita = false;
                else if (evento.keyboard.keycode == ALLEGRO_KEY_A)
                    jogador.movendo_para_esquerda = false;
            }
        }

        /* ----- Desenhando o jogo ----- */

        if (desenhar && al_is_event_queue_empty(fila_eventos))
        {
            // Limpando a tela com uma cor de fundo
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (estado_atual == ESTADO_MENU_PRINCIPAL)
                desenhar_menu(&menu_principal, fonte);

            else if (estado_atual == ESTADO_JOGANDO)
            {
                desenhar_fase(&nivel_1, &cam);
                desenhar_player(&jogador, &cam);

                al_draw_textf(fonte, al_map_rgb(255, 50, 50), 20, 20, ALLEGRO_ALIGN_LEFT, "HP: %d / %d", jogador.hp, jogador.max_hp);
            }
            else if (estado_atual == ESTADO_OPCOES)
            {
                // Tela temporaria para opções
                al_clear_to_color(al_map_rgb(50, 20, 20));
            }
            else if (estado_atual == ESTADO_GAME_OVER)
            {
                // Tela de game over
                al_clear_to_color(al_map_rgb(100, 0, 0));
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "GAME OVER");
                al_draw_text(fonte, al_map_rgb(200, 200, 200), 400, 320, ALLEGRO_ALIGN_CENTER, "Pressione Enter para voltar ao menu");
            }
            else if (estado_atual == ESTADO_VITORIA)
            {
                // Tela de vitória
                al_clear_to_color(al_map_rgb(0, 100, 0));
                al_draw_text(fonte, al_map_rgb(255, 255, 0), 400, 250, ALLEGRO_ALIGN_CENTER, "Fase Concluída!");
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 400, 320, ALLEGRO_ALIGN_CENTER, "Pressione Enter para voltar ao menu");
            }

            // Exibe o que foi desenhado na tela
            al_flip_display();

            // Reseta a variável de desenho
            desenhar = false;
        }
    }

    // Limpando os recursos alocados
    destruir_fase(&nivel_1);
    al_destroy_display(display);
    al_destroy_timer(tempo);
    al_destroy_event_queue(fila_eventos);

    return 0;
}