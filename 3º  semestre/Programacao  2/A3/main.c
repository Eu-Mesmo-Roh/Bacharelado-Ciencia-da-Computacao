#include "motor_grafico.h"
#include "player.h"
#include "mundo.h"
#include "camera.h"
#include "menu.h"
#include "background.h"

const float FPS = 60.0;

void atualizar_resolucao(ALLEGRO_DISPLAY *display)
{
    int w, h;
    float escala_x, escala_y;

    // Calculando a proporção em relação ao tamanho do jogo
    w = al_get_display_width(display);
    h = al_get_display_height(display);

    escala_x = (float)w / 800.0f;
    escala_y = (float)h / 600.0f;

    ALLEGRO_TRANSFORM transforma;
    al_identity_transform(&transforma);
    al_scale_transform(&transforma, escala_x, escala_y);
    // Usando zoom no motor grafico
    al_use_transform(&transforma);
}

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
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    al_set_window_title(display, "2D Souls");

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
    bool pulo_segurado = false;

    // Iniciando o timer
    al_start_timer (tempo);

    // Estado do jogo
    estado_jogo estado_atual = ESTADO_MENU_PRINCIPAL;
    // 0 = continuar, 1 = opções, 2 = Sair
    int opcoes_pausa = 0; 

    //criando o menu
    menu menu_principal;
    iniciar_menu(&menu_principal);

    // Variaveis de opções
    int tecla_esquerda = ALLEGRO_KEY_A;
    int tecla_direita = ALLEGRO_KEY_D;
    int tecla_pulo = ALLEGRO_KEY_W;
    int tecla_agachar = ALLEGRO_KEY_S;
    int tecla_rolar = ALLEGRO_KEY_LSHIFT;

    // 0 = 800x600, 1 = 1024x768, 2 = 1280x720, 3 = 1366x 768
    int resolucao_atual = 0; 
    bool tela_cheia = false;

    int opcao_selecionada = 0;
    bool esperando_tecla = false;


    // Carregando as sprites do cenario e background
    ALLEGRO_BITMAP *img_bg1 = al_load_bitmap("background/BG1.png");
    ALLEGRO_BITMAP *img_bg2 = al_load_bitmap("background/BG2.png");
    ALLEGRO_BITMAP *img_bg3 = al_load_bitmap("background/BG3.png");
    ALLEGRO_BITMAP *tileset = al_load_bitmap("background/Tileset.png");

    ALLEGRO_BITMAP *plataforma_terra = al_create_sub_bitmap(tileset, 2, 0, 41, 32);
    ALLEGRO_BITMAP *plataforma_flutuante = al_create_sub_bitmap(tileset, 2, 0, 40, 20);

    if (!img_bg1 || !img_bg2 || !img_bg3)
    {
        fprintf(stderr, "Erro ao carregar os sprites do cenario e background!\n");
        return -1;
    }

    background bg_parallax;
    iniciar_background(&bg_parallax, img_bg1, img_bg2, img_bg3, 800.0f, 600.0f);

    // Carregando o sprite do player usando o motor gráfico
    float compensacao_y;
    ALLEGRO_BITMAP *sprite_player_parado = al_load_bitmap("assets/_Idle.png");
    ALLEGRO_BITMAP *sprite_player_correndo = al_load_bitmap("assets/_Run.png");
    ALLEGRO_BITMAP *sprite_player_parado_abaixado = al_load_bitmap("assets/_Crouch.png");
    ALLEGRO_BITMAP *sprite_player_andando_abaixado = al_load_bitmap("assets/_CrouchWalk.png");
    ALLEGRO_BITMAP *sprite_player_rolando = al_load_bitmap("assets/_Roll.png");
    ALLEGRO_BITMAP *sprite_player_inverter = al_load_bitmap("assets/_TurnAround.png");
    ALLEGRO_BITMAP *sprite_player_pular = al_load_bitmap("assets/_Jump.png");
    ALLEGRO_BITMAP *sprite_player_cair = al_load_bitmap("assets/_Fall.png");
    ALLEGRO_BITMAP *sprite_player_dano = al_load_bitmap("assets/_Hit.png");
    ALLEGRO_BITMAP *sprite_player_morte = al_load_bitmap("assets/_DeathNoMovement.png");
    
    if (!sprite_player_parado || !sprite_player_correndo || !sprite_player_rolando || !sprite_player_parado_abaixado || !sprite_player_andando_abaixado || !sprite_player_pular || !sprite_player_cair)
    {
        fprintf(stderr, "Erro ao carregar os sprites do player!\n");
        return -1;
    }

    // Carregando os sprites das armadilhas
    ALLEGRO_BITMAP *sprite_suriken = al_load_bitmap("assets_traps/Suriken.png");
    ALLEGRO_BITMAP *sprite_espinho = al_load_bitmap("assets_traps/Spike_B.png");
    ALLEGRO_BITMAP *sprite_lanca = al_load_bitmap("assets_traps/Spear.png");
    ALLEGRO_BITMAP *sprite_fogo_caixa = al_load_bitmap("assets_traps/FireBox.png");
    ALLEGRO_BITMAP *sprite_fogo_chama = al_load_bitmap("assets_traps/Fire.png");
    ALLEGRO_BITMAP *sprite_espinho_movel = al_load_bitmap("assets_traps/Trap_Spike_Run.png");


    if (!sprite_suriken || !sprite_espinho || !sprite_lanca || !sprite_fogo_caixa || !sprite_fogo_chama || !sprite_espinho_movel)
    {
        fprintf(stderr, "Erro ao carregar os sprites das Traps!\n");
        return -1;
    }
    
    // Criando o player
    player jogador;
    iniciar_player(&jogador, 100, 596, 75, 40, sprite_player_parado, sprite_player_correndo, sprite_player_parado_abaixado, sprite_player_andando_abaixado, sprite_player_rolando, sprite_player_inverter, sprite_player_pular, sprite_player_cair, sprite_player_dano, sprite_player_morte);
    
    // Criando o nível
    fase nivel_1;
    iniciar_fase(&nivel_1);
    if (!carregar_mapa(&nivel_1, "fase1.csv", sprite_suriken, sprite_espinho, sprite_lanca, sprite_fogo_caixa, sprite_fogo_chama, sprite_espinho_movel, plataforma_terra, plataforma_flutuante))
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
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            al_acknowledge_resize(display);
            atualizar_resolucao(display);
        }

        // Se o evento inicia o tempo, a lógica do jogo deve ser atualizada
        else if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            if (estado_atual == ESTADO_JOGANDO)
            {

                // Aqui é onde a lógica do jogo será atualizada
                atualizar_player(&jogador, &nivel_1);
                atualizar_fase(&nivel_1);

                // Camera Easter EGG
                if (jogador.x < 0.0f)
                {
                    float destino_x = -800.0f;
                    cam.x += (destino_x - cam.x) * 0.05f;

                    cam.y += (0.0f - cam.y) * 0.05f;
                }
                else
                    atualizar_camera(&cam, &jogador);

                if (jogador.no_chao && !jogador.rolamento && jogador.hp > 0)
                {
                    ALLEGRO_KEYBOARD_STATE estado_teclado;
                    al_get_keyboard_state(&estado_teclado);

                    if (al_key_down(&estado_teclado, tecla_agachar) && !jogador.abaixado)
                    {
                        compensacao_y = 20.0f;

                        jogador.abaixado = true;
                        // Reduz a altura do player pela metade
                        jogador.altura = jogador.altura_original - compensacao_y;
                        // Ajusta a posição y para que o player pareça estar se abaixando
                        jogador.y += compensacao_y;
                    }
                }

                // Se o jogador cair da tela ele morre
                if (jogador.y > 650.0f) 
                    jogador.hp = 0;
                
                if (jogador.morto)
                    estado_atual = ESTADO_GAME_OVER;
                
                // Local da vitoria
                if (jogador.x > 7200.0f) 
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
                if (evento.keyboard.keycode == ALLEGRO_KEY_DELETE)
                    rodando = false;

                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    estado_atual = ESTADO_PAUSADO;
                    opcoes_pausa = 0;

                    // Corrigindo o bug correndo infinito apos o pause
                    jogador.movendo_para_direita = false;
                    jogador.movendo_para_esquerda = false;
                    pulo_segurado = false;

                    if (jogador.abaixado && !jogador.rolamento)
                    {
                        jogador.abaixado = false;
                        jogador.altura = jogador.altura_original;
                        jogador.y -= 20.0f;
                    }
                }
            
                // Jogador começa a se mover para a direita ou esquerda dependendo da tecla precionada
                if (evento.keyboard.keycode == tecla_direita)
                    jogador.movendo_para_direita = true;
                else if (evento.keyboard.keycode == tecla_esquerda)
                    jogador.movendo_para_esquerda = true;
                
                // Jogador pula se a barra de espaço for precionada
                if (evento.keyboard.keycode == tecla_pulo)
                {
                    if (!pulo_segurado)
                    {
                        pular_player(&jogador);
                        pulo_segurado = true;
                    }
                }

                // O jogador se abaixa se a tecla S for precionada, e volta à posição normal quando a tecla for solta
                if (evento.keyboard.keycode == tecla_agachar)
                {
                    if (!jogador.abaixado && !jogador.rolamento && jogador.no_chao && jogador.hp > 0)
                    {
                        if(!jogador.abaixado)
                        {
                            compensacao_y = 20.0f;
    
                            jogador.abaixado = true;
    
                            // Reduz a altura do player pela metade
                            jogador.altura = jogador.altura_original - compensacao_y;
    
                            // Ajusta a posição y para que o player pareça estar se abaixando
                            jogador.y += compensacao_y; 
                        }
                    }
                }

                if (evento.keyboard.keycode == tecla_rolar)
                {
                    // Só rola se não estiver a rolando e se estiver se mexendo
                    if (!jogador.rolamento && jogador.no_chao && (jogador.movendo_para_direita || jogador.movendo_para_esquerda) && jogador.hp > 0)
                    {
                        // Ativa o estado
                        jogador.rolamento = true;

                        // forçando a animação a começar do zero
                        jogador.frame_atual = 0;
                        jogador.timer_animacao = 0;

                        // Aplica o impulso de rolamento
                        if (jogador.movendo_para_esquerda)
                            jogador.vel_x = -9.0f;
                        else if (jogador.movendo_para_direita)
                            jogador.vel_x = 9.0f;

                        // Encolhe a hitbox de forma segura
                        // Verificando se ja esta com um tamanho normal para não encolher mais
                        if (jogador.altura == jogador.altura_original)
                        {   
                            float compensacao_y = 20.0f;

                            // Reduz a altura para o herói passar por debaixo de obstáculos
                            jogador.altura = jogador.altura_original - compensacao_y;

                            // Empurra para baixo para os pés não saírem do chão
                            jogador.y += compensacao_y; 
                        }
                    }
                }
            }

            else if (estado_atual == ESTADO_PAUSADO)
            {
                // Navega para cima
                if (evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    opcoes_pausa--;
                    if (opcoes_pausa < 0) opcoes_pausa = 2; // Loop para o final
                }
                // Navega para baixo
                else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    opcoes_pausa++;
                    if (opcoes_pausa > 2) opcoes_pausa = 0; // Loop para o topo
                }
                // Confirma a opção
                else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    if (opcoes_pausa == 0) {
                        // 1. Continuar
                        estado_atual = ESTADO_JOGANDO;
                    }
                    else if (opcoes_pausa == 1) {
                        // 2. Configurações
                        estado_atual = ESTADO_OPCOES; 
                    }
                    else if (opcoes_pausa == 2) {
                        // 3. Voltar ao Menu Principal
                        
                        // Reseta a vida e as posições do player
                        iniciar_player(&jogador, 100, 596, 75, 40, sprite_player_parado, sprite_player_correndo, sprite_player_parado_abaixado, sprite_player_andando_abaixado, sprite_player_rolando, sprite_player_inverter, sprite_player_pular, sprite_player_cair, sprite_player_dano, sprite_player_morte);
                        
                        // Reseta a câmera para o começo da fase
                        cam.x = 0;
                        cam.y = 0;
                        
                        estado_atual = ESTADO_MENU_PRINCIPAL;
                    }
                }
                // Despausa apertando ESC novamente
                else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    estado_atual = ESTADO_JOGANDO;
                }
            }

            else if (estado_atual == ESTADO_OPCOES)
            {
                if (esperando_tecla)
                {
                    // Se o usuário apertar ESC, cancela a troca de tecla
                    if (evento.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    {
                        if (opcao_selecionada == 0) tecla_esquerda = evento.keyboard.keycode;
                        else if (opcao_selecionada == 1) tecla_direita = evento.keyboard.keycode;
                        else if (opcao_selecionada == 2) tecla_pulo = evento.keyboard.keycode;
                        else if (opcao_selecionada == 3) tecla_agachar = evento.keyboard.keycode;
                        else if (opcao_selecionada == 4) tecla_rolar = evento.keyboard.keycode;
                    }
                    // Terminou de configurar
                    esperando_tecla = false; 
                }
                else
                {
                    // Navegação normal do menu
                    if (evento.keyboard.keycode == ALLEGRO_KEY_W || evento.keyboard.keycode == ALLEGRO_KEY_UP)
                    {
                        opcao_selecionada--;
                        // 8 opções no total (0 a 7)
                        if (opcao_selecionada < 0) 
                            opcao_selecionada = 7; 
                    }
                    else if (evento.keyboard.keycode == ALLEGRO_KEY_S || evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    {
                        opcao_selecionada++;
                        if (opcao_selecionada > 7) 
                            opcao_selecionada = 0;
                    }
                    else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    {
                        if (opcao_selecionada >= 0 && opcao_selecionada <= 4)
                            // Vai trocar um controle
                            esperando_tecla = true;
                        else if (opcao_selecionada == 5)
                        {
                            // Troca a resolução se não estiver em tela cheia
                            resolucao_atual++;
                            if (resolucao_atual > 3) resolucao_atual = 0;

                            int w = 800, h = 600;
                            if (resolucao_atual == 1) { w = 1024; h = 768; }
                            else if (resolucao_atual == 2) { w = 1280; h = 720; }
                            else if (resolucao_atual == 3) { w = 1366; h = 768; }

                            if (!tela_cheia) 
                            {
                                al_resize_display(display, w, h);
                                
                                // Atualiza a resolução apos trocar no menu
                                atualizar_resolucao(display); 
                            }
                        }
                        else if (opcao_selecionada == 6)
                        {
                            // Alterna Tela Cheia
                            tela_cheia = !tela_cheia;
                            al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, tela_cheia);
                            
                            // Se saiu da tela cheia, volta para a resolução da janela que esta marcada
                            if (!tela_cheia) {
                                int w = 800, h = 600;
                                if (resolucao_atual == 1) { w = 1024; h = 768; }
                                else if (resolucao_atual == 2) { w = 1280; h = 720; }
                                else if (resolucao_atual == 3) { w = 1366; h = 768; }
                                al_resize_display(display, w, h);
                            }
                            atualizar_resolucao(display); 
                        }
                        else if (opcao_selecionada == 7)
                        {
                            // Voltar
                            estado_atual = ESTADO_MENU_PRINCIPAL; 
                        }
                    }
                    else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    {
                        estado_atual = ESTADO_MENU_PRINCIPAL;
                    }
                }
            }

            else if (estado_atual == ESTADO_GAME_OVER)
            {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    // Reiniciando o jogo
                    iniciar_player(&jogador, 100, 596, 75, 40, sprite_player_parado, sprite_player_correndo, sprite_player_parado_abaixado, sprite_player_andando_abaixado, sprite_player_rolando, sprite_player_inverter, sprite_player_pular, sprite_player_cair, sprite_player_dano, sprite_player_morte);
                    

                    estado_atual = ESTADO_JOGANDO;
                }
            }

            else if (estado_atual == ESTADO_VITORIA)
            {
                if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    // Reinicia a posição e velocidade para a proxima vez que for jogar
                    iniciar_player(&jogador, 100, 596, 75, 40, sprite_player_parado, sprite_player_correndo, sprite_player_parado_abaixado, sprite_player_andando_abaixado, sprite_player_rolando, sprite_player_inverter, sprite_player_pular, sprite_player_cair, sprite_player_dano, sprite_player_morte);

                    // Voltando para o menu
                    estado_atual = ESTADO_MENU_PRINCIPAL;
                }
            }
        }

        /* ----- Se uma tecla foi solta ----- */
        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (evento.keyboard.keycode == tecla_pulo)
                pulo_segurado = false;

            if (estado_atual == ESTADO_JOGANDO)
            {
                if (evento.keyboard.keycode == tecla_agachar)
                {
                    if (jogador.abaixado)
                    {               
                        // O jogador para de estar abaixado
                        jogador.abaixado = false;

                        // Se o jogador não estiver rolando, ele volta à altura normal
                        if (!jogador.rolamento)
                        {
                            compensacao_y = 20.0f;
                            // Restaura a altura original do player
                            jogador.altura = jogador.altura_original;

                            jogador.y -= compensacao_y; // Ajusta a posição y para que o player pareça estar se levantando
                        }
                    }
                }
                // Jogador para de se mover para a direita ou esquerda dependendo da tecla solta
                if (evento.keyboard.keycode == tecla_direita)
                    jogador.movendo_para_direita = false;
                else if (evento.keyboard.keycode == tecla_esquerda)
                    jogador.movendo_para_esquerda = false;

                if (evento.keyboard.keycode == tecla_rolar)
                {
                    
                }
            }
        }

        /* ----- Desenhando o jogo ----- */

        if (desenhar && al_is_event_queue_empty(fila_eventos))
        {
            // Limpando a tela com uma cor de fundo
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (estado_atual == ESTADO_MENU_PRINCIPAL)
                desenhar_menu(&menu_principal, fonte);

            else if (estado_atual == ESTADO_JOGANDO || estado_atual == ESTADO_PAUSADO)
            {
                // Desenha o jogo normal por baixo
                desenhar_background(&bg_parallax, &cam);
                desenhar_fase(&nivel_1, &cam);
                desenhar_player(&jogador, &cam);

                if (cam.x < -10.0f)
                {
                    // Coloca o texto fixo no centro do espaço secreto (-400) aplicando a câmera
                    al_draw_text(fonte, al_map_rgb(255, 0, 0), -400 - cam.x, 350 - cam.y, ALLEGRO_ALIGN_CENTER, "Feito por Ronaldo");
                    al_draw_text(fonte, al_map_rgb(255, 0, 0), -400 - cam.x, 390 - cam.y, ALLEGRO_ALIGN_CENTER, "Obrigado por jogar!");
                }

                // Janela de Pausa
                if (estado_atual == ESTADO_PAUSADO)
                {
                    // Fundo preto
                    al_draw_filled_rectangle(0, 0, 800, 600, al_map_rgba(0, 0, 0, 180));
                    
                    // Caixa do menu central
                    float caixa_x1 = 200, caixa_y1 = 150, caixa_x2 = 600, caixa_y2 = 450;
                    // Fundo
                    al_draw_filled_rectangle(caixa_x1, caixa_y1, caixa_x2, caixa_y2, al_map_rgb(30, 30, 40)); 
                    // Borda
                    al_draw_rectangle(caixa_x1, caixa_y1, caixa_x2, caixa_y2, al_map_rgb(200, 200, 200), 3.0f); 
                    
                    // Título
                    al_draw_text(fonte, al_map_rgb(255, 200, 0), 400, 180, ALLEGRO_ALIGN_CENTER, "JOGO PAUSADO");
                    al_draw_line(250, 210, 550, 210, al_map_rgb(100, 100, 100), 1.0f); // Linha divisória
                    
                    // Cores para dar feedback visual
                    ALLEGRO_COLOR cor_selecionada = al_map_rgb(0, 255, 255); // Ciano
                    ALLEGRO_COLOR cor_normal = al_map_rgb(150, 150, 150);    // Cinza
                    
                    // Opções de texto
                    al_draw_text(fonte, opcoes_pausa == 0 ? cor_selecionada : cor_normal, 400, 260, ALLEGRO_ALIGN_CENTER, "Continuar");
                    al_draw_text(fonte, opcoes_pausa == 1 ? cor_selecionada : cor_normal, 400, 320, ALLEGRO_ALIGN_CENTER, "Configuracoes");
                    al_draw_text(fonte, opcoes_pausa == 2 ? cor_selecionada : cor_normal, 400, 380, ALLEGRO_ALIGN_CENTER, "Voltar ao Menu Principal");
                    
                    // Desenha o cursor 
                    if (opcoes_pausa == 0) al_draw_text(fonte, cor_selecionada, 250, 260, ALLEGRO_ALIGN_LEFT, ">");
                    if (opcoes_pausa == 1) al_draw_text(fonte, cor_selecionada, 250, 320, ALLEGRO_ALIGN_LEFT, ">");
                    if (opcoes_pausa == 2) al_draw_text(fonte, cor_selecionada, 250, 380, ALLEGRO_ALIGN_LEFT, ">");
                }
            }
            else if (estado_atual == ESTADO_OPCOES)
            {
                // Fundo azul escuro
                al_clear_to_color(al_map_rgb(20, 30, 40)); 

                al_draw_text(fonte, al_map_rgb(255, 200, 0), 400, 50, ALLEGRO_ALIGN_CENTER, "MENU DE OPCOES");
                al_draw_line(200, 80, 600, 80, al_map_rgb(100, 100, 100), 2.0f);

                ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200);
                ALLEGRO_COLOR cor_selecionada = al_map_rgb(0, 255, 255);
                ALLEGRO_COLOR cor_destaque = esperando_tecla ? al_map_rgb(255, 0, 0) : cor_selecionada;

                // Opções de Controles
                al_draw_textf(fonte, opcao_selecionada == 0 ? cor_destaque : cor_normal, 310, 130, ALLEGRO_ALIGN_LEFT, "Mover Esquerda : %s", al_keycode_to_name(tecla_esquerda));
                al_draw_textf(fonte, opcao_selecionada == 1 ? cor_destaque : cor_normal, 310, 180, ALLEGRO_ALIGN_LEFT, "Mover Direita  : %s", al_keycode_to_name(tecla_direita));
                al_draw_textf(fonte, opcao_selecionada == 2 ? cor_destaque : cor_normal, 310, 230, ALLEGRO_ALIGN_LEFT, "Pular          : %s", al_keycode_to_name(tecla_pulo));
                al_draw_textf(fonte, opcao_selecionada == 3 ? cor_destaque : cor_normal, 310, 280, ALLEGRO_ALIGN_LEFT, "Agachar        : %s", al_keycode_to_name(tecla_agachar));
                al_draw_textf(fonte, opcao_selecionada == 4 ? cor_destaque : cor_normal, 310, 330, ALLEGRO_ALIGN_LEFT, "Rolar          : %s", al_keycode_to_name(tecla_rolar));

                // Opção de Resolução
                const char* txt_resolucao = "800x600";
                if (resolucao_atual == 1) 
                    txt_resolucao = "1024x768";
                else if (resolucao_atual == 2) 
                    txt_resolucao = "1280x720";
                else if (resolucao_atual == 3) 
                    txt_resolucao = "1366x768";
                
                al_draw_textf(fonte, opcao_selecionada == 5 ? cor_selecionada : cor_normal, 310, 380, ALLEGRO_ALIGN_LEFT, "Resolucao      : %s", txt_resolucao);
                
                // Tela Cheia
                al_draw_textf(fonte, opcao_selecionada == 6 ? cor_selecionada : cor_normal, 310, 430, ALLEGRO_ALIGN_LEFT, "Tela Cheia     : %s", tela_cheia ? "Sim" : "Nao");

                // Voltar
                al_draw_text(fonte, opcao_selecionada == 7 ? cor_selecionada : cor_normal, 400, 520, ALLEGRO_ALIGN_CENTER, "Voltar");

                // Desenho do Cursor
                if (!esperando_tecla) 
                {
                    float cursor_x = 280; 
                    float cursor_y = 130;
                    
                    if (opcao_selecionada == 0)      { cursor_y = 130; }
                    else if (opcao_selecionada == 1) { cursor_y = 180; }
                    else if (opcao_selecionada == 2) { cursor_y = 230; }
                    else if (opcao_selecionada == 3) { cursor_y = 280; }
                    else if (opcao_selecionada == 4) { cursor_y = 330; }
                    else if (opcao_selecionada == 5) { cursor_y = 380; }
                    else if (opcao_selecionada == 6) { cursor_y = 430; }
                    else if (opcao_selecionada == 7) { cursor_x = 350; cursor_y = 520; }

                    al_draw_text(fonte, cor_selecionada, cursor_x, cursor_y, ALLEGRO_ALIGN_LEFT, ">");
                }

                // Mensagem de aviso quando o jogador for trocar a tecla
                if (esperando_tecla)
                    al_draw_text(fonte, al_map_rgb(255, 100, 100), 400, 475, ALLEGRO_ALIGN_CENTER, "Pressione a nova tecla (ou ESC para cancelar)...");
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

    // Limpando os recursos alocados do mundo e do programa
    destruir_fase(&nivel_1);
    al_destroy_font(fonte);
    al_destroy_display(display);
    al_destroy_timer(tempo);
    al_destroy_event_queue(fila_eventos);

    // limpando os recursos alocados do player
    al_destroy_bitmap(sprite_player_parado);
    al_destroy_bitmap(sprite_player_correndo);
    al_destroy_bitmap(sprite_player_parado_abaixado);
    al_destroy_bitmap(sprite_player_andando_abaixado);
    al_destroy_bitmap(sprite_player_rolando);
    al_destroy_bitmap(sprite_player_inverter);
    al_destroy_bitmap(sprite_player_pular);
    al_destroy_bitmap(sprite_player_cair);
    al_destroy_bitmap(sprite_player_dano);
    al_destroy_bitmap(sprite_player_morte);

    // Limpando os recursos alocados para as armadilhas
    al_destroy_bitmap(sprite_suriken);
    al_destroy_bitmap(sprite_espinho);
    al_destroy_bitmap(sprite_lanca);
    al_destroy_bitmap(sprite_fogo_caixa);
    al_destroy_bitmap(sprite_fogo_chama);
    al_destroy_bitmap(sprite_espinho_movel);

    // Limpando a memoria das imagens do background
    al_destroy_bitmap(img_bg1);
    al_destroy_bitmap(img_bg2);
    al_destroy_bitmap(img_bg3);

    // Limpando a memoria das imagens de cenario
    al_destroy_bitmap(plataforma_terra);
    al_destroy_bitmap(plataforma_flutuante);
    al_destroy_bitmap(tileset);



    return 0;
}