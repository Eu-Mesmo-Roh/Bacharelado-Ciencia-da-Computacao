#include "mundo.h"
#include "camera.h"
#include "motor_grafico.h"

int adicionar_plataforma(fase *f, float x, float y, float largura, float altura, bool invisivel)
{
    plataforma *nova_lista;
    int nova_capacidade, indice_nova_plataforma;

    // Verificando se é necessário aumentar a capacidade do array de  plataformas
    if (f->num_plataformas >= f->capacidade_plataformas)
    {
        if(f->capacidade_plataformas == 0)
            nova_capacidade = 4;
        else
            nova_capacidade = f->capacidade_plataformas * 2;
        
        nova_lista = realloc(f->plataformas, nova_capacidade * sizeof(plataforma));

        if (!nova_lista)
            return 0; // Falha ao alocar memória
        
        f->plataformas = nova_lista;
        f->capacidade_plataformas = nova_capacidade;
    }

    // Adicionando a nova plataforma ao array
    indice_nova_plataforma = f->num_plataformas;
    f->plataformas[indice_nova_plataforma].x = x;
    f->plataformas[indice_nova_plataforma].y = y;
    f->plataformas[indice_nova_plataforma].largura = largura;
    f->plataformas[indice_nova_plataforma].altura = altura;
    f->plataformas[indice_nova_plataforma].invisivel = invisivel;
    f->plataformas[indice_nova_plataforma].vel_x = 0.0f;
    f->plataformas[indice_nova_plataforma].vel_y = 0.0f;
    f->num_plataformas++;
    
    return 1; // Sucesso
}

int adicionar_armadilha_fase(fase *f, float x, float y, float largura, float altura, tipo_armadilha tipo, ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img_secundaria, int max_frames, int largura_frame)
{
    armadilha *nova_lista;
    int nova_capacidade, indice_nova_armadilha;

    if (f->num_armadilhas >= f->capacidade_armadilhas)
    {
        if (f->capacidade_armadilhas == 0)
            nova_capacidade = 4;
        else
            nova_capacidade = f->capacidade_armadilhas * 2;

        nova_lista = realloc(f->armadilhas, nova_capacidade * sizeof(armadilha));

        if (!nova_lista)
            return 0;

        f->armadilhas = nova_lista;
        f->capacidade_armadilhas = nova_capacidade;
    }

    indice_nova_armadilha = f->num_armadilhas;

    iniciar_armadilha(&f->armadilhas[indice_nova_armadilha], x, y, largura, altura, tipo, img, img_secundaria, max_frames, largura_frame);

    f->num_armadilhas++;

    return 1;
}

int carregar_mapa(fase *f, const char *nome_arquivo, ALLEGRO_BITMAP *img_suriken, ALLEGRO_BITMAP *img_espinho, ALLEGRO_BITMAP *img_lanca, ALLEGRO_BITMAP *img_fogo_caixa, ALLEGRO_BITMAP *img_fogo_chama, ALLEGRO_BITMAP *img_bloco)
{
    FILE *mapa;
    char linha[256], *token;
    float x, y, largura, altura;
    int tipo_elemento;
    int contador_linhas;

    mapa = fopen(nome_arquivo, "r");
    if (!mapa)
        return 0; // Falha ao abrir o arquivo
        
    tipo_elemento = -1;
    contador_linhas = 0;

    while (fgets(linha, sizeof(linha), mapa))
    {
        contador_linhas++;

        token = strtok(linha, ", \n\r");
        // Token de tipo de elemento: 0 = platafoma, 1 = armadilha
        if (token == NULL)
            continue;
        
        tipo_elemento = atoi(token);

        // Token para o X do elemento
        token = strtok(NULL, ", \n\r");
        if (token != NULL)
            x = atof(token);
        else
            { printf("DEBUG: Falhou ao ler X na linha %d\n", contador_linhas); continue; }
        
        // Token para o Y do elemento
        token = strtok(NULL, ", \n\r");
        if (token != NULL)
            y = atof(token);
        else 
            { printf("DEBUG: Falhou ao ler Y na linha %d\n", contador_linhas); continue; }
        
        // Token para a largura do elemento
        token = strtok(NULL, ", \n\r");
        if (token != NULL)
            largura = atof(token);
        else
            { printf("DEBUG: Falhou ao ler LARGURA na linha %d\n", contador_linhas); continue; }

        // Token para a altura do elemento e definição do tipo
        token = strtok(NULL, ", \n\r");
        if (token != NULL)
        {
            altura = atof(token);

            printf("DEBUG: Linha %d Carregada com Sucesso -> Tipo: %d | X: %.0f | Y: %.0f | Larg: %.0f | Alt: %.0f\n", 
                    contador_linhas, tipo_elemento, x, y, largura, altura);

            // Decide o que criar baseado no tipo informado CSV
            if (tipo_elemento == 0)
                adicionar_plataforma(f, x, y, largura, altura, false);
            
            // Suriken (8 frames, corte de 32px)
            else if (tipo_elemento == 1)
                adicionar_armadilha_fase(f, x, y, largura, altura, armadilha_suriken, img_suriken, NULL, 8, 32);

            else if (tipo_elemento == 2)
            {
                adicionar_armadilha_fase(f, x, y, largura, altura, armadilha_espinho_estatico, img_espinho, NULL, 1, 32);

                // offsets para a sprite do espinho
                int ultimo = f->num_armadilhas - 1;
                f->armadilhas[ultimo].offset_x = 0.0f;
                f->armadilhas[ultimo].offset_y = 20.0f;
            }

            // lança retratil
            else if (tipo_elemento == 3)
                adicionar_armadilha_fase(f, x, y, largura, altura, armadilha_lanca_retratil, img_lanca, NULL, 13, 16);

            // lança-chamas
            else if (tipo_elemento == 4)
            {
                adicionar_plataforma(f, x, y, largura, altura, true);
                adicionar_armadilha_fase(f, x, y, largura, altura, armadilha_fogo_temporizado, img_fogo_caixa, img_fogo_chama, 14, 16);
            }

            // Bloco movel
            else if (tipo_elemento == 5)
            {
                int largura_frame_bloco = al_get_bitmap_width(img_bloco) / 4;

                adicionar_armadilha_fase(f, x, y, largura, altura, armadilha_bloco_movel, img_bloco, NULL, 4, largura_frame_bloco);

                // offsets para a sprite do bloco movel
                int ultimo = f->num_armadilhas - 1;
                f->armadilhas[ultimo].offset_x = -1.0f;
                f->armadilhas[ultimo].offset_y = -4.0f;

                f->armadilhas[ultimo].vel_x = 1.2f;
            }
        }
        else
        {
            printf("Falha ao extrair o valor de altura na linha %d\n", contador_linhas);
            continue;
        }
    }

    fclose(mapa);

    return 1; // Sucesso
}

int iniciar_fase(fase *f)
{
    // Inicialização do ponteiro das plataformas
    f->plataformas = NULL;
    f->num_plataformas = 0;
    f->capacidade_plataformas = 0;

    //Inicialização do ponteiro das armadilhas
    f->armadilhas = NULL;
    f->num_armadilhas = 0;
    f->capacidade_armadilhas = 0;

    return 1; // Sucesso
}

int atualizar_fase(fase *f)
{
    for (int i = 0; i < f->num_armadilhas; i++)
        atualizar_armadilha(&f->armadilhas[i]);
    
    return 1;
}

int desenhar_fase(fase *f, camera *c)
{
    plataforma *p;
    
    // Desenha as plataformas
    for (int i = 0; i < f->num_plataformas; i++)
    {
        p = &f->plataformas[i];

        // Se for invisível, aciona a visão de Debug!
        if (p->invisivel)
        {
            // bloco de Debug
            al_draw_rectangle(p->x - c->x, p->y - c->y, (p->x + p->largura) - c->x, (p->y + p->altura) - c->y, al_map_rgb(255, 0, 0), 2.0f);
            continue; 
        }

        // Se for normal, desenha verde sólido
        al_draw_filled_rectangle(p->x - c->x, p->y - c->y, (p->x + p->largura) - c->x, (p->y + p->altura) - c->y, al_map_rgb(0, 255, 0));
    }

    // Desenha as armadilhas
    for (int i = 0; i < f->num_armadilhas; i++)
        desenhar_armadilha(&f->armadilhas[i], c);

    return 1; // Sucesso
}

int destruir_fase(fase *f)
{
    // Limpa a memoria das plataformas
    free(f->plataformas);
    f->plataformas = NULL;
    f->num_plataformas = 0;
    f->capacidade_plataformas = 0;

    // Limpa a memória das armadilhas
    free(f->armadilhas);
    f->armadilhas = NULL;
    f->num_armadilhas = 0;
    f->capacidade_armadilhas = 0;

    return 1; // Sucesso
}