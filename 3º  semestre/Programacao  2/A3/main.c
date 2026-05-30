#include "motor_grafico.h"
#include "player.h"
#include "mundo.h"
#include "camera.h"

const float FPS = 60.0;

int main()
{
    // Iniciando o motor gráfico Allegro
    if (!init_motor_grafico())
    {
        fprintf(stderr, "Erro ao inicializar o motor gráfico!\n");
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

    
    // Criando o player
    player jogador;
    iniciar_player(&jogador, 100, 100, 50, 30, false, false);
    
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

        // Se o usuario fechou a janela, o jogo deve ser encerrado
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            rodando = false;
        // Se o evento inicia o tempo, a lógica do jogo deve ser atualizada
        else if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            // Aqui é onde a lógica do jogo será atualizada
            atualizar_player(&jogador, &nivel_1);
            atualizar_camera(&cam, &jogador);
            desenhar = true;
        }
        // Se uma tecla foi precionada
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = false;
        
            // Jogador começa a se mover para a direita ou esquerda dependendo da tecla precionada
            if (evento.keyboard.keycode == ALLEGRO_KEY_D)
                jogador.movendo_para_direita = true;
            else if (evento.keyboard.keycode == ALLEGRO_KEY_A)
                jogador.movendo_para_esquerda = true;
            
            // Jogador pula se a barra de espaço for precionada
            if (evento.keyboard.keycode == ALLEGRO_KEY_W)
                pular_player(&jogador);
        }

        // Se uma tecla foi solta
        else if (evento.type == ALLEGRO_EVENT_KEY_UP)
        {
            // Jogador para de se mover para a direita ou esquerda dependendo da tecla solta
            if (evento.keyboard.keycode == ALLEGRO_KEY_D)
                jogador.movendo_para_direita = false;
            else if (evento.keyboard.keycode == ALLEGRO_KEY_A)
                jogador.movendo_para_esquerda = false;
        }

        /* ----- Desenhando o jogo ----- */

        if (desenhar && al_is_event_queue_empty(fila_eventos))
        {
            // Limpa a tela com a cor preta
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenha o player
            desenhar_player(&jogador, &cam);

            // Desenha o nível
            desenhar_fase(&nivel_1, &cam);

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