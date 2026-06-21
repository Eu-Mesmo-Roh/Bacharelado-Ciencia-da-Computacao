#include "player.h"
#include "mundo.h"
#include "camera.h"

int iniciar_player(player *p, float x, float y, float altura, float largura, ALLEGRO_BITMAP *img_parado, ALLEGRO_BITMAP *img_correndo, ALLEGRO_BITMAP *img_parado_abaixado, ALLEGRO_BITMAP *img_andando_abaixado, ALLEGRO_BITMAP *img_rolando, ALLEGRO_BITMAP *img_inverter, ALLEGRO_BITMAP *img_pular, ALLEGRO_BITMAP *img_cair, ALLEGRO_BITMAP *img_dano, ALLEGRO_BITMAP *img_morte)
{
    // Inicializando as propriedades do player
    p->x = x;
    p->y = y;
    p->altura = altura;
    p->largura = largura;

    // valores padrão de movimento e fisica
    p->vel_x = 0.0f;
    p->vel_y = 0.0f;
    p->no_chao = false;
    p->movendo_para_esquerda = false;
    p->movendo_para_direita = false;
    p->virado_para_esquerda = false;

    // Vida do player
    p->max_hp = 3;
    p->hp = p->max_hp;

    // Outras interações do player
    p->abaixado = false;
    p->altura_original = altura;
    p->rolamento = false;
    p->tempo_rolamento = 0;
    p->jump_buffer = 0;
    p->coyote_time = 0;
    p->timer_invuleravel = 0;
    p->timer_knockback = 0;
    p->tomando_dano = false;
    p->morto = false;

    // Atribui o sprite do player
    p->sprite_parado = img_parado;
    p->sprite_correndo = img_correndo;
    p->sprite_parado_abaixado = img_parado_abaixado;
    p->sprite_andando_abaixado = img_andando_abaixado;
    p->sprite_rolando = img_rolando;
    p->sprite_inverter = img_inverter;
    p->sprite_pular = img_pular;
    p->sprite_cair = img_cair;
    p->sprite_atual = p->sprite_parado;
    p->frame_atual = 0;
    p->timer_animacao = 0;
    p->sprite_dano = img_dano;
    p->sprite_morte = img_morte;

    return 1; // Sucesso
}

int atualizar_player(player *p, fase *f)
{
    plataforma *plataforma_atual;
    armadilha *a;
    ALLEGRO_BITMAP *sprite_anterior = p->sprite_atual;
    int velocidade_frame;
    float velocidade_movimento, compensacao_y, centro_armadilha;
    float hitbox_y_inicio, hitbox_y_fim; 

    // Aplicando a gravidade
    p->vel_y += 0.5f;

    if (p->hp <= 0)
    {
        p->movendo_para_direita = false;
        p->movendo_para_esquerda = false;
        p->jump_buffer = 0;

        if (p->abaixado)
        {
            p->abaixado = false;
            p->altura = p->altura_original;
            p->y -= 20.0f;
        }

    }

    if (p->timer_invuleravel > 0)
        p->timer_invuleravel--;

    if(p->timer_knockback > 0)
    {
        p->timer_knockback--;
        if(p->timer_knockback <= 0)
            p->tomando_dano = false;
    }

    if (p->no_chao)
        p->coyote_time = 6;
    else
    {
        if (p->coyote_time > 0)
            p->coyote_time--;
    }

    if(p->jump_buffer > 0 && p->coyote_time > 0 && !p->rolamento && !p->abaixado)
    {
        p->vel_y = -12.0f; // Impulso de pulo
        p->jump_buffer = 0; // Reseta o buffer de pulo
        p->coyote_time = 0; // Reseta o coyote time
        p->no_chao = false;
    }

    if (p->jump_buffer > 0)
        p->jump_buffer--;

    if (p->hp <= 0)
    {
        // Estado de morte
        p->vel_x *= (p->no_chao ? 0.85f : 0.98f);

        if (p->vel_x > -0.1f && p->vel_x < 0.1f)
            p->vel_x = 0.0f;
    }
    
    else if (!p->tomando_dano)
    {
        if (!p->rolamento)
        {
            // se os dois botoes estiverem precionados ou não ao mesmo tempo para o personagem
            if (p->movendo_para_direita == p->movendo_para_esquerda)
            {
                if (p->vel_x > 0)
                {
                    
                    p->vel_x *= (p->no_chao ? 0.85f : 0.98f);
                    if (p->vel_x < 0.1f) p->vel_x = 0.0f;
                }
                else if (p->vel_x < 0)
                {
                    p->vel_x *= (p->no_chao ? 0.85f : 0.98f);
                    if (p->vel_x > -0.1f) p->vel_x = 0.0f;
                }
            }
    
            // Definindo as velocidades
            else
            {
                velocidade_movimento = p->abaixado ? 2.0f : 5.0f; 
    
                if (p->movendo_para_direita)
                {
                    p->virado_para_esquerda = false;
    
                    if (p->no_chao) 
                    {
                        // Fisica de chão
                        if (p->vel_x < 0.0f) 
                        {
                            p->vel_x += 0.3f;
                            if (p->vel_x >= 0.0f) {
                                p->vel_x = velocidade_movimento;
                            }
                        } 
                        else 
                            p->vel_x = velocidade_movimento;
                        
                    }
                    else 
                        // fisica no ar
                        p->vel_x = velocidade_movimento;
                }
                else if (p->movendo_para_esquerda)
                {
                    p->virado_para_esquerda = true;
    
                    if (p->no_chao) 
                    {
                        if (p->vel_x > 0.0f) 
                        {
                            p->vel_x -= 0.3f;
                            if (p->vel_x <= 0.0f) 
                                p->vel_x = -velocidade_movimento;
                        } else 
                            p->vel_x = -velocidade_movimento;
                    }
                    else 
                        p->vel_x = -velocidade_movimento;
                }
            }
        }
        else
        {
            float velocidade_rolamento = 7.0f;
    
            p->vel_x = p->virado_para_esquerda ? -velocidade_rolamento : velocidade_rolamento;
        }
    }

    // Atualizando a posição em relação ao eixo x do player e construindo sua fisica
    p->x += p->vel_x;

    for (int i = 0; i < f->num_plataformas; i++)
    {
        plataforma_atual = &f->plataformas[i];

        // Verificando colisões horizontais com as plataformas
        if (p->x + p->largura > plataforma_atual->x &&
            p->x < plataforma_atual->x + plataforma_atual->largura &&
            p->y + p->altura > plataforma_atual->y &&
            p->y < plataforma_atual->y + plataforma_atual->altura)
        {
            // Se o player está se movendo para a direita, posiciona o player à esquerda da plataforma
            if (p->vel_x > 0)
                p->x = plataforma_atual->x - p->largura;
            // Se o player está se movendo para a esquerda, posiciona o player à direita da plataforma
            else if (p->vel_x < 0)
                p->x = plataforma_atual->x + plataforma_atual->largura;
            
            // Zera a velocidade horizontal do player
            p->vel_x = 0.0f;
        }
    }

    // Atualizando a posição em relação ao eixo y do player e construindo sua fisica
    p->y += p->vel_y;
    p->no_chao = false;

    for (int i = 0; i < f->num_plataformas; i++)
    {
        plataforma_atual = &f->plataformas[i];

        // Verificando colisões verticais com as plataformas
        if (p->x + p->largura > plataforma_atual->x &&
            p->x < plataforma_atual->x + plataforma_atual->largura &&
            p->y + p->altura > plataforma_atual->y &&
            p->y < plataforma_atual->y + plataforma_atual->altura)
        {
            // Se o player está caindo, posiciona o player em cima da plataforma
            if (p->vel_y > 0)
            {
                p->y = plataforma_atual->y - p->altura;
                p->vel_y = 0.0f;
                p->no_chao = true;
            }
            // Se o player está subindo, posiciona o player embaixo da plataforma
            else if (p->vel_y < 0)
            {
                p->y = plataforma_atual->y + plataforma_atual->altura;
                p->vel_y = 0.0f;

                p->vel_x *= 0.3f;
            }
        }
    }

    /*------Verificando colisão com armadilhas------*/
    for (int i = 0; i < f->num_armadilhas; i++)
    {
        a = &f->armadilhas[i];

        if (!a->estado_ativo)
            continue;
        
        //hitbox padrão
        hitbox_y_inicio = a->y;
        hitbox_y_fim = a->y + a->altura;

        // Se for o lança chamas, o dano é no ar, não na caixa
        if (a->tipo == armadilha_fogo_temporizado)
        {
            hitbox_y_inicio = a->y - a->altura;
            hitbox_y_fim = a->y;
        }

        // testando colisão com as hitbox
        if (p->x + p->largura > a->x && p->x < a->x + a->largura &&
            p->y + p->altura > hitbox_y_inicio && p->y < hitbox_y_fim)
        {
            if (!p->rolamento)
            {
                // Encontrar o centro da armadilha para saber o lado ao qual empurrar o player
                centro_armadilha = a->x + (a->largura / 2.0f);
    
                // Ativa a função de dano, e knockback
                dar_dano_player(p, centro_armadilha);
            }
        }
    }

    /*------Maquina de estados visual------*/
    if (p->hp <= 0)
        p->sprite_atual = p->sprite_morte;
    else if (p->tomando_dano)
        p->sprite_atual = p->sprite_dano;
    else if (p->rolamento)
        p->sprite_atual = p->sprite_rolando;
    
    else if (!p->no_chao)
    {
        if (p->vel_y < 0)
            p->sprite_atual = p->sprite_pular;
        else
            p->sprite_atual = p->sprite_cair;
    }

    else if(p->abaixado)
    {
        if(p->vel_x != 0.0f)
            p->sprite_atual = p->sprite_andando_abaixado;
        else
            p->sprite_atual = p->sprite_parado_abaixado;
    }
    else if (p->no_chao && ((p->movendo_para_direita && p->vel_x < 0.0f) || (p->movendo_para_esquerda && p->vel_x > 0.0f) || (p->vel_x != 0.0f && p->movendo_para_direita == p->movendo_para_esquerda)))
        p->sprite_atual = p->sprite_inverter;
    else if (p->vel_x != 0.0f)
        p->sprite_atual = p->sprite_correndo;
    else
        p->sprite_atual = p->sprite_parado;

    if(p->sprite_atual != sprite_anterior)
    {
        p->frame_atual = 0;
        p->timer_animacao = 0;
    }
    
    /*------Motor de animação------*/
    p->timer_animacao++;

    if (p->rolamento) 
        velocidade_frame = 4; // A velocidade rápida que definiu para o rolamento
    else if (p->sprite_atual == p->sprite_correndo) 
        velocidade_frame = 5; // <-- Reduza este valor! (Teste 4, 5 ou 6). O padrão era 10.
    else if (p->sprite_atual == p->sprite_inverter)
    {
        if (p->movendo_para_direita == p->movendo_para_esquerda)
            {
                velocidade_frame = 999;
                p->frame_atual = 0; 
            }
        else
            velocidade_frame = 5; // Velocidade para a animação de escorregando para os dois lados
    }
    else if (p->sprite_atual == p->sprite_pular || p->sprite_atual == p->sprite_cair)
        velocidade_frame = 5; // Velocidade para as animações de pular e cair (sem animação, apenas um frame)
    else if (p->sprite_atual == p->sprite_parado_abaixado)
        velocidade_frame = 1; // Velocidade para o sprite parado abaixado (sem animação, apenas um frame)
    else if (p->sprite_atual == p->sprite_dano)
        velocidade_frame = 10;
    else if (p->sprite_atual == p->sprite_morte)
        velocidade_frame = 8;
    else if (p->sprite_atual == p->sprite_andando_abaixado)
        velocidade_frame = 6; // Pode querer que o rastejo seja um pouco mais rápido também
    else 
        velocidade_frame = 10; // Velocidade lenta e suave para quando está Parado (Idle) ou Agachado Parado

    // Ajuste o valor para controlar a velocidade da animação
    if (p->timer_animacao >= velocidade_frame) 
    {
        p->timer_animacao = 0;
        p->frame_atual++;

        // Número de frames na animação para correr e parado
        int max_frames = 10;

        if(p->sprite_atual == p->sprite_rolando)
            max_frames = 12; // Número de frames para a animação de rolamento
        else if(p->sprite_atual == p->sprite_parado_abaixado)
            max_frames = 1; // numero de frames para o sprite parado abaixado
        else if(p->sprite_atual == p->sprite_andando_abaixado)
            max_frames = 8; // Número de frames para a animação de andar abaixado
        else if (p->sprite_atual == p->sprite_inverter)
            max_frames = 3; // Número de frames para a animação de escorregando para os dois lados
        else if (p->sprite_atual == p->sprite_pular || p->sprite_atual == p->sprite_cair)
            max_frames = 3; // Número de frames para as animações de pular e cair
        else if (p->sprite_atual == p->sprite_dano)
            max_frames = 1;
        else if (p->sprite_atual == p->sprite_morte)
            max_frames = 10;

        if(p->frame_atual >= max_frames)
        {
            if (p->sprite_atual == p->sprite_morte)
            {
                p->frame_atual = max_frames - 1;
                p->morto = true;
            }
            else if (p->rolamento)
            {
                p->rolamento = false;
                p->vel_x = 0.0f; // Para o movimento horizontal após o rolamento

                if(!p->abaixado)
                {
                    if (p->altura != p->altura_original)
                    {
                        // Restaura a altura original do player
                        compensacao_y = p->altura_original - p->altura;
                        p->altura = p->altura_original;
                        p->y -= compensacao_y; // Ajusta a posição y para que o player
                    }
                }

                p->frame_atual = max_frames - 1; // Mantém o último frame da animação de rolamento
            }
            else if(p->sprite_atual == p->sprite_parado_abaixado || p->sprite_atual == p->sprite_inverter || p->sprite_atual == p->sprite_pular || p->sprite_atual == p->sprite_cair)
                p->frame_atual = max_frames - 1; // Mantém o último frame da animação de rolamento
            else
                p->frame_atual = 0; // Reinicia a animação para os outros
        }
    }



    return 1; // Sucesso
}

int pular_player(player *p)
{
    // Se acabou a vida, não registra pulo
    if (p->hp <= 0)
        return 0;

    // Se estiver fazendo uma ação guarda a intenção de pulo
    if (!p->rolamento && !p->abaixado)
    {
        p->jump_buffer = 6; // Guarda o comando por 6 frames
    }
    return 1; // Sucesso
}

int dar_dano_player(player *p, float x_dano)
{
    // So toma dano se não estiver invulnerável
    if (p->timer_invuleravel <= 0)
    {
        p->hp--;
        p->timer_invuleravel = 90;
        p->timer_knockback = 27;
        p->tomando_dano = true;

        // Aplica um impulso de knockback
        p->vel_y = -6.0f;

        // Empurrando para a direção oposta
        if (p->x < x_dano)
        {
            p->vel_x = -5.0f; // Empurra para a esquerda
            p->virado_para_esquerda = false; // Virado para a direita
        }
        else
        {
            p->vel_x = 5.0f; // Empurra para a direita
            p->virado_para_esquerda = true; // Virado para a esquerda
        }
    }

    return 1;
}

int desenhar_player(player *p, camera *c)
{
    int flags;
    float escala, offset_x, offset_y, pos_x_hud, pos_y_hud, largura_maxima, altura_barra, porcentagem_vida, largura_atual;
    bool desenhar_invertido;

    if(p->sprite_atual)
    {

        flags = 0;
        // Fator de escala para aumentar o tamanho do sprite
        escala = 2.0f; 

        offset_x = 0.0f; // Variável para ajustar o posicionamento horizontal do sprite
        offset_y = 0.0f; // Variável para ajustar o posicionamento vertical do sprite

        desenhar_invertido = p->virado_para_esquerda;

        if (p->sprite_atual == p->sprite_inverter)
        {
            if (p->vel_x > 0.0f)
                desenhar_invertido = false;
            else if (p->vel_x < 0.0f)
                desenhar_invertido = true;
        }

        if (desenhar_invertido)
            // Sprite invertido para a esquerda
            flags = ALLEGRO_FLIP_HORIZONTAL;

        if(p->rolamento)
        {
            // Ajusta o offset para o sprite de rolamento
            offset_x = p->virado_para_esquerda ? 107.0f : 94.0f; // Puxa o desenho para esquerda
            offset_y = 105.0f; // Puxa o desenho para cima
        }
        else if(p->abaixado)
        {
            // Ajusta o offset para o sprite abaixado
            offset_x = p->virado_para_esquerda ? 111.0f : 89.0f; // Puxa o desenho para esquerda
            offset_y = 105.0f; // Puxa o desenho para cima
        }
        else if (p->sprite_atual == p->sprite_inverter)
        {
            // Ajusta o offset para o sprite de escorregando para os dois lados
            offset_x = p->virado_para_esquerda ? 100.0f : 100.0f; // Puxa o desenho para esquerda
            offset_y = 85.0f; // Puxa o desenho para cima
        }
        else if (p->sprite_atual == p->sprite_pular || p->sprite_atual == p->sprite_cair)
        {
            // Ajusta o offset para os sprites de pular e cair
            offset_x = p->virado_para_esquerda ? 111.0f : 89.0f; // Puxa o desenho para esquerda
            offset_y = 85.0f; // Puxa o desenho para cima
        }
        else
        {
            // Padrão parado
            offset_x = p->virado_para_esquerda ? 111.0f : 89.0f; // Puxa o desenho para esquerda
            offset_y = 85.0f; // Puxa o desenho para cima    
        }

        // Supondo que cada frame tenha 80 pixels de largura
        int pos_x_recorte = p->frame_atual * 120; 

        ALLEGRO_COLOR cor_hitbox;
        if (p->rolamento)
            cor_hitbox = al_map_rgb(0, 0, 255);
        else
            cor_hitbox = al_map_rgb(255, 0, 0);

        al_draw_rectangle(p->x - c->x, p->y - c->y, p->x + p->largura - c->x, p->y + p->altura - c->y, cor_hitbox, 2.0f);

        if(p->hp <= 0 || p->tomando_dano || p->timer_invuleravel <= 0 || p->timer_invuleravel % 4 < 2) // Pisca o sprite quando estiver invulnerável
            al_draw_scaled_bitmap(p->sprite_atual, pos_x_recorte, 0, 120, 80, (p->x - c->x) - offset_x, (p->y - c->y) - offset_y, 120 * escala, 80 * escala, flags);
    }

    // Posição fixa da barra de vida
    pos_x_hud = 20.0f;
    pos_y_hud = 20.0f;
    largura_maxima = 150.0f;
    altura_barra = 20.0f;

    // Calcula a porcentagem de vida para saber o quanto pintar
    porcentagem_vida = (float)p->hp / (float)p->max_hp;
    if(porcentagem_vida < 0.0f)
        porcentagem_vida = 0.0f;
    
    largura_atual = largura_maxima * porcentagem_vida;

    // Fundo da barra de vida
    al_draw_filled_rectangle(pos_x_hud, pos_y_hud, pos_x_hud + largura_maxima, pos_y_hud + altura_barra, al_map_rgb(50, 50, 50));

    // Preenchimento da barra de vida
    if (p->hp > 0)
        al_draw_filled_rectangle(pos_x_hud, pos_y_hud, pos_x_hud + largura_atual, pos_y_hud + altura_barra, al_map_rgb(220, 20, 20));
    
    // Contorno da barra de vida
    al_draw_rectangle(pos_x_hud, pos_y_hud, pos_x_hud + largura_maxima, pos_y_hud + altura_barra, al_map_rgb(255, 255, 255), 2.0f);

    return 1; // Sucesso
}