#include "mundo.h"
#include "camera.h"
#include "motor_grafico.h"

int carregar_mapa(fase *f, const char *nome_arquivo)
{
    FILE *mapa;
    char linha[256], *token;
    float x, y, largura, altura;
    int contador_linhas = 0;

    mapa = fopen(nome_arquivo, "r");

    if (!mapa)
        return 0; // Falha ao abrir o arquivo
    
    while (fgets(linha, sizeof(linha), mapa))
    {
        contador_linhas++;

        token = strtok(linha, ", \n\r");

        if (token != NULL)
        {
            x = atof(token);

            token = strtok(NULL, ", \n\r");
            if (token != NULL)
                y = atof(token);
            else
            {
                printf(" Falha ao extrair o valor de y na linha %d\n", contador_linhas);
                continue;
            }

            token = strtok(NULL, ", \n\r");
            if (token != NULL)
                largura = atof(token);
            else
            {
                printf(" Falha ao extrair o valor de largura na linha %d\n", contador_linhas);
                continue;
            }

            token = strtok(NULL, ", \n\r");
            if (token != NULL)
            {
                altura = atof(token);

                adicionar_plataforma(f, x, y, largura, altura);
            }
            else
            {
                printf(" Falha ao extrair o valor de altura na linha %d\n", contador_linhas);
                continue;
            }
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

int adicionar_plataforma(fase *f, float x, float y, float largura, float altura)
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
    f->plataformas[indice_nova_plataforma].vel_x = 0.0f;
    f->plataformas[indice_nova_plataforma].vel_y = 0.0f;
    f->num_plataformas++;
    
    return 1; // Sucesso
}

int adicionar_armadilha_fase(fase *f, float x, float y, float largura, float altura, tipo_armadilha tipo, ALLEGRO_BITMAP *img, int max_frames, int largura_frame)
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

    iniciar_armadilha(&f->armadilhas[indice_nova_armadilha], x, y, largura, altura, tipo, img, max_frames, largura_frame);

    f->num_armadilhas++;

    return 1;
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