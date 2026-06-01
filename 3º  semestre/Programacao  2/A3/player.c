#include "player.h"
#include "mundo.h"
#include "camera.h"

int iniciar_player(player *p, float x, float y, float altura, float largura, ALLEGRO_BITMAP *sprite)
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

    // Vida do player
    p->max_hp = 3;
    p->hp = p->max_hp;

    // Outras interações do player
    p->abaixado = false;
    p->altura_original = altura;
    p->rolamento = false;
    p->tempo_rolamento = 0;

    // Atribui o sprite do player
    p->sprite = sprite; 
    p->frame_atual = 0;
    p->timer_animação = 0;

    return 1; // Sucesso
}

int atualizar_player(player *p, fase *f)
{
    plataforma *plataforma_atual;

    // Aplicando a gravidade
    p->vel_y += 0.5f;

    // Se o player não está se movendo para nenhuma direção, reduzimos a velocidade gradualmente para simular atrito
    if (p->movendo_para_direita == p->movendo_para_esquerda)
    {
        if (p->vel_x > 0)
        {
            // Reduzindo a velocidade para a direita
            p->vel_x *= 0.85f;
            if (p->vel_x < 0.1f)
                p->vel_x = 0.0f;
        }
        else if (p->vel_x < 0)
        {
            // Reduzindo a velocidade para a esquerda
            p->vel_x *= 0.85f;
            if (p->vel_x > -0.1f)
                p->vel_x = 0.0f;
        }
    }

    if (!p->rolamento)
    {
        // O player se move mais devagar quando está abaixado
        float velocidade_movimento = p->abaixado ? 3.0f : 5.0f; 

        if (p->movendo_para_direita)
            p->vel_x = velocidade_movimento;
        else if (p->movendo_para_esquerda)
            p->vel_x = -velocidade_movimento;
        else
            p->vel_x = 0.0f;
    }

    // Presumimos que o player não está no chão até verificarmos as colisões
    p->no_chao = false;
    
    // Verificando colisões com as plataformas
    for(int i = 0; i < f->num_plataformas; i++)
    {
        plataforma_atual = &f->plataformas[i];

        // Verificando se o player está colidindo com a plataforma atual
        if (p->x + p->largura > plataforma_atual->x &&
            p->x < plataforma_atual->x + plataforma_atual->largura &&
            p->y + p->altura > plataforma_atual->y &&
            p->y < plataforma_atual->y + plataforma_atual->altura)
        {
            if (p->vel_y > 0)
            {
                // Posiciona o player em cima da plataforma
                p->y = plataforma_atual->y - p->altura; 
                
                // Zera a velocidade vertical do player
                p->vel_y = 0.0f;

                // O player está no chão
                p->no_chao = true;
            }
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
            }
        }
    }

    p->timer_animação++;
    if (p->timer_animação >= 10) // Ajuste o valor para controlar a velocidade da animação
    {
        p->timer_animação = 0;
        p->frame_atual++;

        if(p->frame_atual >= 10) // Supondo que o sprite tenha 4 frames de animação
            p->frame_atual = 0;
    }


    return 1; // Sucesso
}

int pular_player(player *p)
{
    // O player só pode pular se estiver no chão
    if (p->no_chao && !p->rolamento && !p->abaixado)
    {
        // Aplicando um impulso para cima
        p->vel_y = -12.0f;
        p->no_chao = false;
    }
    return 1; // Sucesso
}

int desenhar_player(player *p, camera *c)
{
    if(p->sprite)
    {
        int flags = 0;
        
        if (p->movendo_para_esquerda)
        // Sprite invertido para a esquerda
        flags = ALLEGRO_FLIP_HORIZONTAL;


        // Supondo que cada frame tenha 80 pixels de largura
        int pos_x_recorte = p->frame_atual * 120; 

        // Fator de escala para aumentar o tamanho do sprite
        float escala = 2.0f; 

        al_draw_scaled_bitmap(p->sprite, pos_x_recorte, 0, 120, 80, p->x - c->x, p->y - c->y, 120 * escala, 80 * escala, flags);
    }

    return 1; // Sucesso
}