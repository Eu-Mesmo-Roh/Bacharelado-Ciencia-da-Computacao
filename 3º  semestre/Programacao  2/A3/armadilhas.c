#include "armadilhas.h"

void iniciar_armadilha(armadilha *a, float x, float y, float largura, float altura, tipo_armadilha tipo, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img_secundaria, int max_frames, int largura_frame)
{
    a->x = x;
    a->y = y;
    a->largura = largura;
    a->altura = altura;
    a->tipo = tipo;
    a->sprite = img;
    a->sprite_secundario = img_secundaria;
    a->max_frames = max_frames;
    a->largura_frame = largura_frame;

    a->offset_x = 0.0f;
    a->offset_y = 0.0f;
    
    // Inicialização de estados especificos
    a->frame_atual = 0;
    a->frame_secundario = 0;
    a->timer_estado = 0;
    a->timer_animacao = 0;
    a->estado_ativo = false;
    
    // Configuração padrão de movimento
    a->vel_x = 2.0f;
    a->velocidade_animacao = 3;

    // Inicialização do bloco movel
    a->x_inicial = x;
    a->limite_patrulha = 150.0f;
}

void atualizar_armadilha(armadilha *a)
{
    a->timer_estado++;

    switch (a->tipo)
    {
        case armadilha_suriken:
        {
            // Ativando a colisão da suriken
            a->estado_ativo = true;

            // Giro continuo da suriken
            a->timer_animacao++;
            if (a->timer_animacao >= 3)
            {
                a->timer_animacao = 0;
                a->frame_atual = (a->frame_atual + 1) % a->max_frames;
            }
            break;
        }

        case armadilha_espinho_estatico:
        {
            a->estado_ativo = true;
            break;
        }
        
        case armadilha_lanca_retratil:
        {
            // A Tabela de Pesquisa: de frames
            static const int ordem_frames[] = 
            {
                // Fase de Espera
                0, 0, 0, 0, 0, 0, 0, 0, 0,
                // Fase de Aviso
                1, 0, 1, 0, 1,
                // Fase de Disparo
                2, 3, 4,
                // Fase esticada
                5, 8, 9, 10, 11, 5,
                // Fase de retorno
                6, 7
            };

            // Definindo a duração total da animação
            int total_passos = 25; // Quantidade de itens no nosso array
            int ciclo_total = 200;
            
            // Calculando o índice atual
            int passo_atual = (a->timer_estado * total_passos) / ciclo_total;
            
            // Trava de segurança
            if (passo_atual >= total_passos) 
                passo_atual = total_passos - 1;

            // Atribui o frame baseado na tabela
            a->frame_atual = ordem_frames[passo_atual];

            // Lógica de Dano
            // A lança só mata quando está esticada
            if (passo_atual >= 15 && passo_atual <= 24)
                a->estado_ativo = true;
            else
                a->estado_ativo = false;

            // Reset do Ciclo
            a->timer_estado++;
            if (a->timer_estado >= ciclo_total) a->timer_estado = 0;

            break;
        }
        
        case armadilha_fogo_temporizado:
        {
            // Array para a animação da caixa lança chamas
            static const int ordem_frames[] = 
            {
                // Aviso: Caixa fechada
                0, 1, 2, 3,   0, 1, 2, 3,   0, 1, 2, 3,  
                
                // Abrindo a caixa
                8, 8, 9, 10, 11, 12,             
                
                // Liberando a carga até o fim
                4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7
            };
            
            // Temos exatamente 30 números no array
            int total_passos = 30; 
            
            // CORREÇÃO: 180 ticks / 30 passos = 6 ticks por frame.
            int passo_atual = a->timer_estado / 6; 
            
            // Trava de segurança no último índice do array
            if (passo_atual >= total_passos) passo_atual = total_passos - 1; 

            // Aplica o frame visual da caixa
            a->frame_atual = ordem_frames[passo_atual];

            // Controle do Fogo começando no frame 18 e indo até 0 30
            if (passo_atual >= 18 && passo_atual < 30) 
            {
                a->estado_ativo = true; // Queima!
                
                // Roda a animação independente das chamas
                a->timer_animacao++;
                if (a->timer_animacao >= 3) {
                    a->timer_animacao = 0;
                    a->frame_secundario = (a->frame_secundario + 1) % 4; 
                }
            } 
            else 
                // Seguro, o fogo apagou
                a->estado_ativo = false; 

            // Reseta para o próximo ciclo
            if (a->timer_estado >= 180) 
                a->timer_estado = 0; 
            
            break;
        }

        case armadilha_bloco_movel:
        {
            a->estado_ativo = true;

            // Movimentação horizontal
            a->x += a->vel_x;

            // Inverte a direção ao bater no limite
            if (a->x > a->x_inicial + a->limite_patrulha)
            {
                a->x = a->x_inicial + a->limite_patrulha;
                a->vel_x = -a->vel_x;
            }
            else if (a->x < a->x_inicial)
            {
                // Garantindo que não passa do ponto de origem
                a->x = a->x_inicial;
                a->vel_x = -a->vel_x;
            }

            a->timer_animacao++;
            if (a->timer_animacao >= 10)
            {
                a->timer_animacao = 0;

                a->frame_atual = (a->frame_atual + 1) % a->max_frames;
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

void desenhar_armadilha(armadilha *a, camera *c)
{
    int w, h, frame_x, pos_x_recorte, largura_fogo, pos_x_fogo, flag_espelho;
    float proporcao, altura_ajustada, draw_y_ajustado, escala, altura_desenho, draw_y_padrao, altura_fogo, draw_y_fogo;

    if (!a->sprite) 
        return;

    // Posicionamento base com câmera e offsets
    float draw_x = (a->x - c->x) + a->offset_x;

    // Desenho especifico do espinho estatico
    if (a->tipo == armadilha_espinho_estatico)
    {
        w = a->largura_frame;
        h = al_get_bitmap_height(a->sprite);
        frame_x = 6;

        proporcao = a->largura / w;
        altura_ajustada = h * proporcao;
        draw_y_ajustado = (a->y +a->altura) - altura_ajustada - c->y + a->offset_y;
        
        al_draw_scaled_bitmap(a->sprite, frame_x * w, 0, w, h, draw_x, draw_y_ajustado, a->largura, altura_ajustada, 0);
    }
    
    // Desenho especifico do bloco movel
    // Desenho especifico do bloco movel
    else if (a->tipo == armadilha_bloco_movel)
    {
        w = a->largura_frame;
        h = 32; // Mantemos a altura cravada em 32 para pegar só a primeira linha
        pos_x_recorte = a->frame_atual * w;

        // --- O TRUQUE MÁGICO: ZOOM ---
        // Aumente este valor (ex: 1.5f, 1.8f, 2.0f) para a imagem crescer 
        // até o espinho sólido encostar na borda da caixa amarela!
        float zoom = 1.1f; 

        proporcao = a->largura / w;
        
        // Calculamos o tamanho visual expandido
        float draw_w = a->largura * zoom;
        float draw_h = (h * proporcao) * zoom;

        // Calculamos o recuo para garantir que o zoom aconteça a partir do centro da hitbox
        float ajuste_centro_x = (draw_w - a->largura) / 2.0f;
        float ajuste_centro_y = (draw_h - a->altura) / 2.0f;

        // Posições finais na tela já com a câmera, o offset manual e o ajuste do zoom
        float final_draw_x = (a->x - c->x) - ajuste_centro_x + a->offset_x;
        float final_draw_y = (a->y - c->y) - ajuste_centro_y + a->offset_y;
        
        flag_espelho = (a->vel_x < 0) ? ALLEGRO_FLIP_HORIZONTAL : 0;

        al_draw_scaled_bitmap(a->sprite, pos_x_recorte, 0, w, h, 
                            final_draw_x, final_draw_y, 
                            draw_w, draw_h, flag_espelho);
    }

    // Desenho padrao para as outras armadilhas
    else
    {
        escala = a->largura / a->largura_frame;
        altura_desenho = al_get_bitmap_height(a->sprite) * escala;
        
        // Alinhamento pela base da hitbox
        draw_y_padrao = ((a->y + a->altura) - altura_desenho - c->y) + a->offset_y;
        pos_x_recorte = a->frame_atual * a->largura_frame;
    
        al_draw_scaled_bitmap(a->sprite, pos_x_recorte, 0, a->largura_frame, al_get_bitmap_height(a->sprite), 
                             draw_x, draw_y_padrao, a->largura_frame * escala, altura_desenho, 0);
    }

    // Desenho Secundário (Lança-chamas)
    if (a->tipo == armadilha_fogo_temporizado && a->estado_ativo && a->sprite_secundario)
    {
        largura_fogo = 16;
        pos_x_fogo = a->frame_secundario * largura_fogo;
        altura_fogo = al_get_bitmap_height(a->sprite_secundario) * escala;
        draw_y_fogo = (a->y - c->y) - altura_fogo;

        al_draw_scaled_bitmap(a->sprite_secundario, pos_x_fogo, 0, 
                            largura_fogo, al_get_bitmap_height(a->sprite_secundario), 
                            (a->x - c->x), draw_y_fogo, 
                            largura_fogo * escala, altura_fogo, 0);
    }

    // Debug Hitbox
    al_draw_rectangle(a->x - c->x, a->y - c->y, 
                    a->x + a->largura - c->x, a->y + a->altura - c->y, 
                    al_map_rgb(255, 255, 0), 1.0f);
}